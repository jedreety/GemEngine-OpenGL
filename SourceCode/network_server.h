#pragma once

#include <enet/enet.h>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <atomic>
#include <glm/glm.hpp>

class NetworkServer {
public:
    NetworkServer(uint16_t port);
    ~NetworkServer();

    void Start();
    void Stop();

private:
    void Run();

    ENetAddress address_;
    ENetHost* server_;
    std::thread serverThread_;
    std::atomic<bool> running_;

    std::mutex mutex_;
    // Map of client peer IDs to their positions
    std::unordered_map<enet_uint32, glm::vec3> clientPositions_;
};