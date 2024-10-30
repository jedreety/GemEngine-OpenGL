#include "network_endpoint.h"
#include <iostream>

namespace Networking {

    NetworkEndpoint::NetworkEndpoint()
        : host_(nullptr), running_(false) {
    }

    NetworkEndpoint::~NetworkEndpoint() {
        Stop();
        if (host_) {
            enet_host_destroy(host_);
            host_ = nullptr;
        }
        enet_deinitialize();
    }

    bool NetworkEndpoint::InitializeENet() {
        if (enet_initialize() != 0) {
            std::cerr << "An error occurred while initializing ENet." << std::endl;
            return false;
        }
        return true;
    }

    void NetworkEndpoint::Stop() {
        running_ = false;
        if (networkThread_.joinable()) {
            networkThread_.join();
        }
    }

    void NetworkEndpoint::SendMessage(ENetPeer* peer, const ENetPacket* packet) {
        if (peer && packet) {
            enet_peer_send(peer, 0, const_cast<ENetPacket*>(packet));
        }
    }

} // namespace Networking
