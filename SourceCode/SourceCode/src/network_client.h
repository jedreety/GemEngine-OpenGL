#pragma once

#include <enet/enet.h>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <atomic>
#include <glm/glm.hpp>

class NetworkClient {
public:
    NetworkClient(const std::string& serverAddress, uint16_t port);
    ~NetworkClient();

    void Start();
    void Stop();

    // Send player's position to the server
    void SendPosition(const glm::vec3& position);

    // Get positions of other players
    std::unordered_map<enet_uint32, glm::vec3> GetOtherPlayersPositions();

private:
    void Run();

    ENetHost* client_;
    ENetPeer* serverPeer_;
    std::thread clientThread_;
    std::atomic<bool> running_;

    std::mutex mutex_;
    // Map of other players' positions
    std::unordered_map<enet_uint32, glm::vec3> otherPlayersPositions_;
};
