#include "network_messageFactory.h"
#include "network_MTposition.h"
#include "network_MTchat.h"
#include "network_MTping.h"
#include "network_MTpong.h"
#include "network_MTclientID.h"
#include <cstring>

namespace Networking {

    std::unique_ptr<Message> MessageFactory::CreateMessage(const uint8_t* data, size_t size) {
        if (size < sizeof(MessageType)) {
            return nullptr;
        }

        MessageType type;
        std::memcpy(&type, data, sizeof(MessageType));

        std::unique_ptr<Message> message;

        switch (type) {
        case MessageType::PositionUpdate:
            message = std::make_unique<PositionUpdateMessage>();
            break;
        case MessageType::ChatMessage:
            message = std::make_unique<ChatMessage>();
            break;
        case MessageType::Ping:
            message = std::make_unique<PingMessage>();
            break;
        case MessageType::Pong:
            message = std::make_unique<PongMessage>();
            break;
        case MessageType::ClientID:
            message = std::make_unique<ClientIDMessage>();
            break;
            // Add other message types here...
        default:
            return nullptr;
        }

        if (!message->Deserialize(data, size)) {
            return nullptr;
        }

        return message;
    }

} // namespace Networking
