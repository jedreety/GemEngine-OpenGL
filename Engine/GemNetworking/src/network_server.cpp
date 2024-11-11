// network_server.cpp

#include <Gem/Networking/network_server.h>
#include <iostream>
#include <cstring>
#include <chrono>

namespace Network {

    // Constructor
    Server::Server(uint16_t port) : server_(nullptr), running_(false) {
        // Initialize ENet
        if (enet_initialize() != 0) {
            std::cerr << "An error occurred while initializing ENet." << std::endl;
            exit(EXIT_FAILURE);
        }

        // Set up the address to listen on
        address_.host = ENET_HOST_ANY;
        // Set the port
        address_.port = port;

        // Create the ENet server host
        server_ = enet_host_create(&address_, /* max clients */ 32, /* channels */ 2, 0, 0);

        if (server_ == nullptr) {
            std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Destructor
    Server::~Server() {
        Stop();
        // Destroy the server host
        if (server_ != nullptr) {
            enet_host_destroy(server_);
        }
        // Deinitialize ENet
        enet_deinitialize();
    }

    // Start the server thread
    void Server::Start() {
        running_ = true;
        // Start the server thread
        serverThread_ = std::thread(&Server::Run, this);
    }

    // Stop the server thread
    void Server::Stop() {
        running_ = false;
        // Wait for the server thread to finish
        if (serverThread_.joinable()) {
            // Join the server thread
            serverThread_.join();
        }
    }

    void Server::Run() {

        while (running_) {

            ENetEvent event;

            // Wait up to 10 milliseconds for an event
            while (enet_host_service(server_, &event, 10) > 0) {

                // Lock the mutex to protect shared data
                std::lock_guard<std::mutex> lock(mutex_);

                switch (event.type) {

                case ENET_EVENT_TYPE_CONNECT:

                    // Initialize the client's position
                    clientPositions_[event.peer->connectID] = glm::vec3(0.0f);
                    break;

                case ENET_EVENT_TYPE_RECEIVE:

                    // Check if the packet is a glm::vec3
                    if (event.packet->dataLength == sizeof(glm::vec3)) {

                        // Update the client's position
                        glm::vec3 position;
                        std::memcpy(&position, event.packet->data, sizeof(glm::vec3));
                        clientPositions_[event.peer->connectID] = position;

                        // Broadcast the position to other clients
                        for (size_t i = 0; i < server_->peerCount; ++i) {

                            // Retrieve the client
                            ENetPeer& client = server_->peers[i];

                            // Skip the sender and disconnected clients
                            if (client.state != ENET_PEER_STATE_CONNECTED || client.connectID == event.peer->connectID) {
                                continue;
                            }

                            // Prepare packet with sender ID and position

                            // Packet size is the size of an enet_uint32 (client ID) plus the size of a glm::vec3 (position)
                            size_t packetSize = sizeof(enet_uint32) + sizeof(glm::vec3);
                            // Create a packet with the specified data size and flags
                            ENetPacket* packet = enet_packet_create(nullptr, packetSize, ENET_PACKET_FLAG_RELIABLE);
                            // Copy the client ID and position into the packet data
                            std::memcpy(packet->data, &event.peer->connectID, sizeof(enet_uint32));
                            // Copy the position into the packet data
                            std::memcpy(packet->data + sizeof(enet_uint32), &position, sizeof(glm::vec3));
                            // Send the packet to the client
                            enet_peer_send(&client, 0, packet);

                            // debug
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
                    // Reset the client's data
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
}