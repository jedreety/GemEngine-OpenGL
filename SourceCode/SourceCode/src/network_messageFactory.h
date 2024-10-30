#pragma once

#include "network_message.h"
#include <cstdint>
#include <memory>

namespace Networking {

    class MessageFactory {
    public:
        static std::unique_ptr<Message> CreateMessage(const uint8_t* data, size_t size);
    };

} // namespace Networking
