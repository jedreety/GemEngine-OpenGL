#pragma once

#include "Side.h"
#include "Quad.h"
#include <glm/glm.hpp>

namespace Engine {

    class Face {
    public:
        Face(const Side& side, const Quad& quad) noexcept : side_(side), quad_(&quad) {}

        // Generate positions for the face's vertices
        std::array<std::array<float, 3>, 4> positions(float voxelSize, const glm::vec3& offset) const;

        // Generate normals for the face
        std::array<std::array<float, 3>, 4> normals() const noexcept;

        // Generate texture coordinates for the face
        std::array<std::array<float, 2>, 4> uvs(bool flipU, bool flipV) const noexcept;


        const Side& side() const noexcept { return side_; }
        const Quad& quad() const noexcept { return *quad_; }

    private:
        Side side_;
        const Quad* quad_;
    };

}
