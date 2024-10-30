#pragma once

#include "network_message.h"

namespace Networking {

    class PingMessage : public Message {
    public:
        PingMessage();

        MessageType GetType() const override;
        std::vector<uint8_t> Serialize() const override;
        bool Deserialize(const uint8_t* data, size_t size) override;
    };

} // namespace Networking
