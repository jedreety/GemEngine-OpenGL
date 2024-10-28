// network_client.cpp

#include "network_client.h"
#include <iostream>
#include <cstring>
#include <chrono>

// Constructor
NetworkClient::NetworkClient(const std::string& serverAddress, uint16_t port)
    : client_(nullptr), serverPeer_(nullptr), running_(false) {
    // Initialize ENet
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create a client host
    client_ = enet_host_create(nullptr, 1, 2, 0, 0);

    if (client_ == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet client host." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    ENetAddress address;
    enet_address_set_host(&address, serverAddress.c_str());
    address.port = port;

    serverPeer_ = enet_host_connect(client_, &address, 2, 0);

    if (serverPeer_ == nullptr) {
        std::cerr << "No available peers for initiating an ENet connection." << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Destructor
NetworkClient::~NetworkClient() {
    Stop();
    if (client_ != nullptr) {
        enet_host_destroy(client_);
    }
    enet_deinitialize();
}

// Start the client thread
void NetworkClient::Start() {
    running_ = true;
    clientThread_ = std::thread(&NetworkClient::Run, this);
}

// Stop the client thread
void NetworkClient::Stop() {
    running_ = false;
    if (clientThread_.joinable()) {
        clientThread_.join();
    }
}

// Send player's position to the server
void NetworkClient::SendPosition(const glm::vec3& position) {
    ENetPacket* packet = enet_packet_create(&position, sizeof(glm::vec3), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(serverPeer_, 0, packet);

    std::cout << "Client " << client_->peers->connectID << " sent position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
}

// Get positions of other players
std::unordered_map<enet_uint32, glm::vec3> NetworkClient::GetOtherPlayersPositions() {
    std::lock_guard<std::mutex> lock(mutex_);
    return otherPlayersPositions_;
}

// Client main loop
void NetworkClient::Run() {
    while (running_) {
        ENetEvent event;
        // Wait up to 10 milliseconds for an event
        while (enet_host_service(client_, &event, 10) > 0) {
            std::lock_guard<std::mutex> lock(mutex_);
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "Connected to server." << std::endl;
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                if (event.packet->dataLength == sizeof(enet_uint32) + sizeof(glm::vec3)) {
                    // Extract sender ID and position
                    enet_uint32 senderID;
                    glm::vec3 position;

                    std::memcpy(&senderID, event.packet->data, sizeof(enet_uint32));
                    std::memcpy(&position, event.packet->data + sizeof(enet_uint32), sizeof(glm::vec3));

                    // Update other player's position
                    otherPlayersPositions_[senderID] = position;

                    std::cout << "Client received position from client " << senderID << ": "
                        << position.x << ", " << position.y << ", " << position.z << std::endl;
                }
                // Destroy the received packet
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnected from server." << std::endl;
                running_ = false;
                break;

            default:
                break;
            }
        }
        // Sleep briefly to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
