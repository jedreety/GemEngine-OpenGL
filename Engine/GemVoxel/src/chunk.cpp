#include <Gem/Voxel/chunk.h>
#include <algorithm>
#include <cstdint>

namespace Gem {

	namespace Voxel {

		Chunk::Chunk() {

			// Fill the array with Voxel::Default()
			std::fill(std::begin(voxels_), std::end(voxels_), Voxel());
		}

		Voxel Chunk::get_voxel(GLuint x, GLuint y, GLuint z) {

			return voxels_[Chunk::linearize(x, y, z)];
		}

		GLuint Chunk::linearize(GLuint x, GLuint y, GLuint z) {
			return x + (y * length_) + (z * area_);
		}

		std::tuple<GLuint, GLuint, GLuint> Chunk::delinearize(GLuint index) {
			GLuint z = index / (area_);
			index -= z * (area_);

			GLuint y = index / length_;
			index -= y * length_;

			// GLuint x = index;

			return std::make_tuple(index, y, z);

		}

		auto Chunk::get_length() {
			
			return length_;
		}

		auto Chunk::get_area() {
				
			return area_;
		}
		auto Chunk::get_volume() {
				
			return volume_;
		}


	} // namespace Voxel

} // namesapce Gem