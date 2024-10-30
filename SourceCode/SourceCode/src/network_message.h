#pragma once

#include <vector>
#include <cstdint>
#include <memory>

namespace Networking {

    class Message {
    public:
        virtual ~Message(){};

        virtual MessageType GetType() const = 0;
        virtual std::vector<uint8_t> Serialize() const = 0;
        virtual bool Deserialize(const uint8_t* data, size_t size) = 0;
    };

} // namespace Networking
