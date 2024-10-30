#pragma once

#include "network_message.h"
#include <glm/glm.hpp>
#include <enet/enet.h>
#include <cstdint>

namespace Networking {

    class PositionUpdateMessage : public Message {
    public:
        enet_uint32 clientID_;
        glm::vec3 position_;

        PositionUpdateMessage();
        PositionUpdateMessage(enet_uint32 clientID, const glm::vec3& position);

        MessageType GetType() const override;
        std::vector<uint8_t> Serialize() const override;
        bool Deserialize(const uint8_t* data, size_t size) override;
    };

} // namespace Networking
