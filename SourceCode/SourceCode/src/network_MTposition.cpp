#include "network_MTposition.h"
#include <cstring>

namespace Networking {

    PositionUpdateMessage::PositionUpdateMessage()
        : clientID_(0), position_(0.0f) {}

    PositionUpdateMessage::PositionUpdateMessage(enet_uint32 clientID, const glm::vec3& position)
        : clientID_(clientID), position_(position) {}

    MessageType PositionUpdateMessage::GetType() const {
        return MessageType::PositionUpdate;
    }

    std::vector<uint8_t> PositionUpdateMessage::Serialize() const {
        std::vector<uint8_t> outData(sizeof(MessageType) + sizeof(enet_uint32) + sizeof(float) * 3);
        size_t offset = 0;
        MessageType type = GetType();
        std::memcpy(outData.data() + offset, &type, sizeof(MessageType));
        offset += sizeof(MessageType);
        std::memcpy(outData.data() + offset, &clientID_, sizeof(enet_uint32));
        offset += sizeof(enet_uint32);
        std::memcpy(outData.data() + offset, &position_, sizeof(float) * 3);
        return outData;
    }

    bool PositionUpdateMessage::Deserialize(const uint8_t* data, size_t size) {
        if (size < sizeof(MessageType) + sizeof(enet_uint32) + sizeof(float) * 3) return false;
        size_t offset = 0;
        offset += sizeof(MessageType); // Skip type as it's already known
        std::memcpy(&clientID_, data + offset, sizeof(enet_uint32));
        offset += sizeof(enet_uint32);
        std::memcpy(&position_, data + offset, sizeof(float) * 3);
        return true;
    }

} // namespace Networking
