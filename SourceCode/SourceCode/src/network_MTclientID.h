#pragma once

#include "network_message.h"
#include <enet/enet.h>
#include <cstdint>

namespace Networking {

    class ClientIDMessage : public Message {
    public:
        enet_uint32 clientID_;

        ClientIDMessage();
        ClientIDMessage(enet_uint32 clientID);

        MessageType GetType() const override;
        void Serialize(std::vector<uint8_t>& outData) const override;
        bool Deserialize(const uint8_t* data, size_t size) override;
    };

} // namespace Networking
