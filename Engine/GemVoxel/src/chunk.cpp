#include <Gem/Voxel/chunk.h>
#include <algorithm>

namespace Gem {
    namespace Voxel {

        Chunk::Chunk() {
            // Initialize all voxels with default constructor
            voxels_.fill(Voxel());
        }

        Voxel& Chunk::getVoxel(uint32_t x, uint32_t y, uint32_t z) {
            size_t index = linearize(x, y, z);
            return voxels_.at(index);
        }

        void Chunk::setVoxel(uint32_t x, uint32_t y, uint32_t z, const Voxel& voxel) {
            size_t index = linearize(x, y, z);
            voxels_.at(index) = voxel;
        }

        constexpr size_t Chunk::linearize(uint32_t x, uint32_t y, uint32_t z) {
            if (x >= length_ || y >= length_ || z >= length_) {
                throw std::out_of_range("Coordinates out of bounds in linearize.");
            }
            return x + y * length_ + z * area_;
        }

        std::tuple<uint32_t, uint32_t, uint32_t> Chunk::delinearize(size_t index) {
            if (index >= volume_) {
                throw std::out_of_range("Index out of bounds in delinearize.");
            }

            uint32_t z = index / area_;
            index %= area_;

            uint32_t y = index / length_;

            uint32_t x = index % length_;

            return std::make_tuple(x, y, z);
        }

        Voxel& Chunk::operator()(uint32_t x, uint32_t y, uint32_t z) {
            return getVoxel(x, y, z);
        }

    } // namespace Voxel
} // namespace Gem
