#include "network_MTpong.h"
#include <cstring>

namespace Networking {

    PongMessage::PongMessage() {}

    MessageType PongMessage::GetType() const {
        return MessageType::Pong;
    }

    void PongMessage::Serialize(std::vector<uint8_t>& outData) const {
        outData.resize(sizeof(MessageType));
        MessageType type = GetType();
        std::memcpy(outData.data(), &type, sizeof(MessageType));
    }

    bool PongMessage::Deserialize(const uint8_t* data, size_t size) {
        return size == sizeof(MessageType);
    }

} // namespace Networking
