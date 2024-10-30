#include "network_MTclientID.h"
#include <cstring>

namespace Networking {

    ClientIDMessage::ClientIDMessage()
        : clientID_(0) {}

    ClientIDMessage::ClientIDMessage(enet_uint32 clientID)
        : clientID_(clientID) {}

    MessageType ClientIDMessage::GetType() const {
        return MessageType::ClientIDMessage;
    }

    void ClientIDMessage::Serialize(std::vector<uint8_t>& outData) const {
        outData.resize(sizeof(MessageType) + sizeof(enet_uint32));
        size_t offset = 0;
        MessageType type = GetType();
        std::memcpy(outData.data() + offset, &type, sizeof(MessageType));
        offset += sizeof(MessageType);
        std::memcpy(outData.data() + offset, &clientID_, sizeof(enet_uint32));
    }

    bool ClientIDMessage::Deserialize(const uint8_t* data, size_t size) {
        if (size < sizeof(MessageType) + sizeof(enet_uint32)) return false;
        size_t offset = 0;
        offset += sizeof(MessageType); // Skip type as it's already known
        std::memcpy(&clientID_, data + offset, sizeof(enet_uint32));
        return true;
    }

} // namespace Networking
