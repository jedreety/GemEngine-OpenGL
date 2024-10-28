// network_server.cpp

#include "network_server.h"
#include <iostream>
#include <cstring>
#include <chrono>

// Constructor
NetworkServer::NetworkServer(uint16_t port) : server_(nullptr), running_(false) {
    // Initialize ENet
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set up the address to listen on
    address_.host = ENET_HOST_ANY;
    address_.port = port;

    // Create the ENet server host
    server_ = enet_host_create(&address_, /* max clients */ 32, /* channels */ 2, 0, 0);

    if (server_ == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Destructor
NetworkServer::~NetworkServer() {
    Stop();
    if (server_ != nullptr) {
        enet_host_destroy(server_);
    }
    enet_deinitialize();
}

// Start the server thread
void NetworkServer::Start() {
    running_ = true;
    serverThread_ = std::thread(&NetworkServer::Run, this);
}

// Stop the server thread
void NetworkServer::Stop() {
    running_ = false;
    if (serverThread_.joinable()) {
        serverThread_.join();
    }
}

void NetworkServer::Run() {
    while (running_) {
        ENetEvent event;
        // Wait up to 10 milliseconds for an event
        while (enet_host_service(server_, &event, 10) > 0) {
            std::lock_guard<std::mutex> lock(mutex_);
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "A new client connected from "
                    << event.peer->address.host << ":" << event.peer->address.port << "." << std::endl;
                // Initialize the client's position
                clientPositions_[event.peer->connectID] = glm::vec3(0.0f);
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                if (event.packet->dataLength == sizeof(glm::vec3)) {
                    // Update the client's position
                    glm::vec3 position;
                    std::memcpy(&position, event.packet->data, sizeof(glm::vec3));
                    clientPositions_[event.peer->connectID] = position;

                    std::cout << "Server received position from client " << event.peer->connectID << ": "
                        << position.x << ", " << position.y << ", " << position.z << std::endl;


                    // Broadcast the position to other clients
                    for (size_t i = 0; i < server_->peerCount; ++i) {
                        ENetPeer& client = server_->peers[i];
                        if (client.state != ENET_PEER_STATE_CONNECTED || client.connectID == event.peer->connectID) {
                            continue;
                        }
                        // Prepare packet with sender ID and position
                        size_t packetSize = sizeof(enet_uint32) + sizeof(glm::vec3);
                        ENetPacket* packet = enet_packet_create(nullptr, packetSize, ENET_PACKET_FLAG_RELIABLE);
                        std::memcpy(packet->data, &event.peer->connectID, sizeof(enet_uint32));
                        std::memcpy(packet->data + sizeof(enet_uint32), &position, sizeof(glm::vec3));
                        enet_peer_send(&client, 0, packet);

                        // Inside the loop, after enet_peer_send()
                        std::cout << "Server sent position of client " << event.peer->connectID << " to client " << client.connectID << std::endl;

                    }
                }
                // Destroy the received packet
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Client disconnected: " << event.peer->connectID << std::endl;
                // Remove the client's position
                clientPositions_.erase(event.peer->connectID);
                event.peer->data = nullptr;
                break;

            default:
                break;
            }
        }
        // Sleep briefly to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

