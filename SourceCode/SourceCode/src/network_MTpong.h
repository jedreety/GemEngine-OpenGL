#pragma once

#include "network_message.h"

namespace Networking {

    class PongMessage : public Message {
    public:
        PongMessage();

        MessageType GetType() const override;
        void Serialize(std::vector<uint8_t>& outData) const override;
        bool Deserialize(const uint8_t* data, size_t size) override;
    };

} // namespace Networking
