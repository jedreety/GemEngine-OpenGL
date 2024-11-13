#pragma once

#include <GlfwGlad.h>


namespace Gem {

	namespace Voxel {

		const uint8_t CHUNK_BOUNDARY = 16;

		class Voxel {};

		class Chunk {

		public:


			Chunk();

			GLuint linearize(GLuint x, GLuint y, GLuint z);

			std::tuple<GLuint, GLuint, GLuint> delinearize(GLuint index);

			Voxel get_voxel(GLuint x, GLuint y, GLuint z);

			auto get_length();
			auto get_area();
			auto get_volume();

		private:
			static const uint8_t length_ = CHUNK_BOUNDARY;
			static const auto area_ = CHUNK_BOUNDARY * CHUNK_BOUNDARY;
			static const auto volume_ = CHUNK_BOUNDARY * CHUNK_BOUNDARY * CHUNK_BOUNDARY;

			Voxel voxels_[volume_];

		};

	} // namespace Voxel

} // namesapce Gem