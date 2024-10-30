#include "network_MTclientID.h"
#include <cstring>

namespace Networking {

    ClientIDMessage::ClientIDMessage()
        : clientID_(0), position_(0.0f) {
    }

    ClientIDMessage::ClientIDMessage(uint32_t clientID)
        : clientID_(clientID) {
        printf("%d\n", clientID_);
    }

    MessageType ClientIDMessage::GetType() const {
        return MessageType::PositionUpdate;
    }

    std::vector<uint8_t> ClientIDMessage::Serialize() const {
        std::vector<uint8_t> data(sizeof(MessageType) + sizeof(clientID_) + sizeof(position_));
        uint8_t* ptr = data.data();

        // Serialize MessageType
        MessageType type = GetType();
        std::memcpy(ptr, &type, sizeof(MessageType));
        ptr += sizeof(MessageType);

        // Serialize clientID_
        std::memcpy(ptr, &clientID_, sizeof(clientID_));
        ptr += sizeof(clientID_);

        // Serialize position_
        std::memcpy(ptr, &position_, sizeof(position_));

        return data;
    }

    void ClientIDMessage::Deserialize(const uint8_t* data, size_t size) {
        const uint8_t* ptr = data;

        // Skip MessageType (already known)
        ptr += sizeof(MessageType);

        // Deserialize clientID_
        std::memcpy(&clientID_, ptr, sizeof(clientID_));
        ptr += sizeof(clientID_);

        // Deserialize position_
        std::memcpy(&position_, ptr, sizeof(position_));
    }

} // namespace Networking
