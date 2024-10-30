#include "network_MTping.h"
#include <cstring>

namespace Networking {

    PingMessage::PingMessage() {}

    MessageType PingMessage::GetType() const {
        return MessageType::Ping;
    }

    std::vector<uint8_t> PingMessage::Serialize() const {
        outData.resize(sizeof(MessageType));
        MessageType type = GetType();
        std::memcpy(outData.data(), &type, sizeof(MessageType));
    }

    bool PingMessage::Deserialize(const uint8_t* data, size_t size) {
        return size == sizeof(MessageType);
    }

} // namespace Networking
