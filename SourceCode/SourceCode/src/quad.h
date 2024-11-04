#pragma once

#include <array>
#include <cstddef>

namespace Engine {

    class Quad {
    public:
        // Constructor to initialize the Quad with voxel position and texture layer
        constexpr Quad(const std::array<std::size_t, 3>& voxel, uint32_t type_layer) noexcept
            : voxel_(voxel), type_layer_(type_layer) {}

        // Getters for voxel position and texture layer
        constexpr const std::array<std::size_t, 3>& voxel() const noexcept { return voxel_; }
        constexpr uint32_t type_layer() const noexcept { return type_layer_; }

    private:
        std::array<std::size_t, 3> voxel_; // Voxel position within the chunk
        uint32_t type_layer_;              // Texture layer index
    };

}