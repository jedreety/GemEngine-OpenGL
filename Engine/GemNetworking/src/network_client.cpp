// network_client.cpp

#include <Gem/Networking/network_client.h>
#include <iostream>
#include <cstring>
#include <chrono>

namespace Network {

    // Constructor
    Client::Client(const std::string& serverAddress, uint16_t port)
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

        // Adress of the server
        ENetAddress address;
        // Set the server address
        enet_address_set_host(&address, serverAddress.c_str());
        // Set the server port
        address.port = port;
        // Connect to the server
        serverPeer_ = enet_host_connect(client_, &address, 2, 0);

        if (serverPeer_ == nullptr) {
            std::cerr << "No available peers for initiating an ENet connection." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Destructor
    Client::~Client() {

        Stop();

        // Destroy the client host
        if (client_ != nullptr) {
            enet_host_destroy(client_);
        }
        // Deinitialize ENet
        enet_deinitialize();
    }

    // Start the client thread
    void Client::Start() {

        running_ = true;

        // Start the client thread
        clientThread_ = std::thread(&Client::Run, this);
    }

    // Stop the client thread
    void Client::Stop() {

        running_ = false;

        // Wait for the client thread to finish
        if (clientThread_.joinable()) {
            // Join the client thread
            clientThread_.join();
        }
    }

    // Send player's position to the server
    void Client::SendPosition(const glm::vec3& position) {

        // Create a packet with the position data
        ENetPacket* packet = enet_packet_create(&position, sizeof(glm::vec3), ENET_PACKET_FLAG_RELIABLE);

        // Send the packet to the server
        enet_peer_send(serverPeer_, 0, packet);

    }

    // Get positions of other players
    std::unordered_map<enet_uint32, glm::vec3> Client::GetOtherPlayersPositions() {

        // Lock the mutex to protect shared data
        std::lock_guard<std::mutex> lock(mutex_);

        // debug
        otherPlayersPositions_[0] = glm::vec3(0.0f);

        return otherPlayersPositions_;
    }

    // Client main loop
    void Client::Run() {

        while (running_) {

            ENetEvent event;

            // Wait up to 10 milliseconds for an event
            while (enet_host_service(client_, &event, 10) > 0) {

                // Lock the mutex to protect shared data
                std::lock_guard<std::mutex> lock(mutex_);

                switch (event.type) {

                case ENET_EVENT_TYPE_CONNECT:

                    std::cout << "Connected to server." << std::endl;
                    break;

                case ENET_EVENT_TYPE_RECEIVE:

                    // Check if the packet is a glm::vec3 with an enet_uint32
                    if (event.packet->dataLength == sizeof(enet_uint32) + sizeof(glm::vec3)) {

                        // Extract sender ID and position
                        enet_uint32 senderID;
                        glm::vec3 position;

                        std::memcpy(&senderID, event.packet->data, sizeof(enet_uint32));
                        std::memcpy(&position, event.packet->data + sizeof(enet_uint32), sizeof(glm::vec3));

                        // Update other player's position
                        otherPlayersPositions_[senderID] = position;

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
} // namespace Network