#pragma once

#include <GlfwGlad.h>
#include <array>
#include <tuple>
#include <stdexcept>

namespace Gem {
    namespace Voxel {

        constexpr uint8_t CHUNK_BOUNDARY = 16;

        class Voxel {
            // Assume Voxel has some meaningful implementation
        };

        class Chunk {
        public:
            Chunk();

            /**
             * @brief Retrieves the voxel at the specified coordinates.
             * @param x The x-coordinate.
             * @param y The y-coordinate.
             * @param z The z-coordinate.
             * @return Reference to the Voxel at the specified position.
             * @throws std::out_of_range if coordinates are out of bounds.
             */
            Voxel& getVoxel(uint32_t x, uint32_t y, uint32_t z);

            /**
             * @brief Sets the voxel at the specified coordinates.
             * @param x The x-coordinate.
             * @param y The y-coordinate.
             * @param z The z-coordinate.
             * @param voxel The voxel to set.
             * @throws std::out_of_range if coordinates are out of bounds.
             */
            void setVoxel(uint32_t x, uint32_t y, uint32_t z, const Voxel& voxel);

            /**
             * @brief Converts 3D coordinates to a linear index.
             * @param x The x-coordinate.
             * @param y The y-coordinate.
             * @param z The z-coordinate.
             * @return The linear index.
             * @throws std::out_of_range if coordinates are out of bounds.
             */
            static constexpr size_t linearize(uint32_t x, uint32_t y, uint32_t z);

            /**
             * @brief Converts a linear index to 3D coordinates.
             * @param index The linear index.
             * @return A tuple containing x, y, z coordinates.
             * @throws std::out_of_range if index is out of bounds.
             */
            static std::tuple<uint32_t, uint32_t, uint32_t> delinearize(size_t index);

            // Accessors for chunk dimensions
            static constexpr uint32_t getLength() { return length_; }
            static constexpr uint32_t getArea() { return area_; }
            static constexpr uint32_t getVolume() { return volume_; }

            /**
             * @brief Overloads the function call operator to access voxels.
             * @param x The x-coordinate.
             * @param y The y-coordinate.
             * @param z The z-coordinate.
             * @return Reference to the Voxel at the specified position.
             * @throws std::out_of_range if coordinates are out of bounds.
             */
            Voxel& operator()(uint32_t x, uint32_t y, uint32_t z);

        private:
            static constexpr uint32_t length_ = CHUNK_BOUNDARY;
            static constexpr uint32_t area_ = length_ * length_;
            static constexpr uint32_t volume_ = area_ * length_;

            std::array<Voxel, volume_> voxels_;
        };

    } // namespace Voxel
} // namespace Gem
