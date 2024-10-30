#pragma once

#include <cstdint>

namespace Networking {

    enum class MessageType : uint8_t {
        ClientID = 0,
        PositionUpdate,
        ChatMessage,
        Ping,
        Pong,
        // Add more message types as needed
    };

} // namespace Networking
