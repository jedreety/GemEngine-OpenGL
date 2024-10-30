#pragma once

#include "network_message.h"
#include <string>
#include <cstdint>
#include <enet/enet.h>

namespace Networking {

    class ChatMessage : public Message {
    public:
        enet_uint32 senderID_;
        std::string message_;

        ChatMessage();
        ChatMessage(enet_uint32 senderID, const std::string& message);

        MessageType GetType() const override;
        std::vector<uint8_t> Serialize() const override;
        bool Deserialize(const uint8_t* data, size_t size) override;
    };

} // namespace Networking
