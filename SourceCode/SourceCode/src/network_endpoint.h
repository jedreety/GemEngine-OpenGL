#pragma once

#include <enet/enet.h>

namespace Networking {

    class NetworkEndpoint {
    public:
        virtual ~NetworkEndpoint() {}

        virtual void Start() = 0;

    protected:
        virtual void Run() = 0;
        virtual void HandleEvent(const ENetEvent& event) = 0;
    };

} // namespace Networking
