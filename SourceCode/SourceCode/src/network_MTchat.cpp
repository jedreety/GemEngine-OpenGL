#include "network_MTchat.h"
#include <cstring>

namespace Networking {

    ChatMessage::ChatMessage()
        : senderID_(0), message_("") {}

    ChatMessage::ChatMessage(enet_uint32 senderID, const std::string& message)
        : senderID_(senderID), message_(message) {}

    MessageType ChatMessage::GetType() const {
        return MessageType::ChatMessage;
    }

    std::vector<uint8_t> ChatMessage::Serialize() const {
        uint32_t messageLength = static_cast<uint32_t>(message_.size());
        std::vector<uint8_t> outData(sizeof(MessageType) + sizeof(enet_uint32) + sizeof(uint32_t) + messageLength);
        size_t offset = 0;
        MessageType type = GetType();
        std::memcpy(outData.data() + offset, &type, sizeof(MessageType));
        offset += sizeof(MessageType);
        std::memcpy(outData.data() + offset, &senderID_, sizeof(enet_uint32));
        offset += sizeof(enet_uint32);
        std::memcpy(outData.data() + offset, &messageLength, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        std::memcpy(outData.data() + offset, message_.data(), messageLength);
        return outData;
    }

    bool ChatMessage::Deserialize(const uint8_t* data, size_t size) {
        if (size < sizeof(MessageType) + sizeof(enet_uint32) + sizeof(uint32_t)) return false;
        size_t offset = 0;
        offset += sizeof(MessageType); // Skip type
        std::memcpy(&senderID_, data + offset, sizeof(enet_uint32));
        offset += sizeof(enet_uint32);
        uint32_t messageLength = 0;
        std::memcpy(&messageLength, data + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        if (size < offset + messageLength) return false;
        message_.assign(reinterpret_cast<const char*>(data + offset), messageLength);
        return true;
    }

} // namespace Networking
