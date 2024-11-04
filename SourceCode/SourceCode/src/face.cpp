#include "face.h"

namespace Engine {

    std::array<std::array<float, 3>, 4> Face::positions(float voxelSize, const glm::vec3& offset) const {
        // Convert voxel position to float and apply voxelSize and offset
        float voxelX = static_cast<float>(quad_->voxel()[0]) * voxelSize + offset.x;
        float voxelY = static_cast<float>(quad_->voxel()[1]) * voxelSize + offset.y;
        float voxelZ = static_cast<float>(quad_->voxel()[2]) * voxelSize + offset.z;

        // Initialize array to hold the positions
        std::array<std::array<float, 3>, 4> positions;

        // Determine the face normal axis index
        int axis = static_cast<int>(side_.get_axis());

        // Determine the positive offset along the normal axis
        float normalOffset = side_.get_positive() ? voxelSize : 0.0f;

        // Axes perpendicular to the face normal
        int axisU = (axis + 1) % 3;
        int axisV = (axis + 2) % 3;

        // Precompute offsets along U and V axes
        float offsets[2] = { 0.0f, voxelSize };

        // Generate corner positions
        for (int i = 0; i < 4; ++i) {
            std::array<float, 3> corner = { voxelX, voxelY, voxelZ };

            // Set the position along the normal axis
            corner[axis] += normalOffset;

            // Set positions along U and V axes using bitmasking
            corner[axisU] += offsets[(i >> 0) & 1];
            corner[axisV] += offsets[(i >> 1) & 1];

            positions[i] = corner;
        }

        return positions;
    }



    std::array<std::array<float, 3>, 4> Face::normals() const noexcept {
        std::array<float, 3> normal = side_.normal();
        return { normal, normal, normal, normal };
    }

    std::array<std::array<float, 2>, 4> Face::uvs(bool flipU, bool flipV) const noexcept {
        if (flipU && flipV) {
            return { { {1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f} } };
        }
        else if (flipU) {
            return { { {1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} } };
        }
        else if (flipV) {
            return { { {0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f} } };
        }
        else {
            return { { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} } };
        }
    }
}
