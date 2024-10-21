#pragma once

#include "glad/glad.h"
#include <cstdint> // for fixed-width integer types
#include <list>
#include <string>
#include <algorithm> // for std::find
#include <iostream>
#include <stb/stb_image.h>

typedef uint8_t u8;

namespace Engine 
{
	namespace Graphics
	{
		namespace Texture {
		
			class TextureManager {
			public:

				TextureManager();
				void Init();
				~TextureManager();

				// Check if the windows attributes are set
				inline bool is_attr_set() const;

				void generate();


				void Bind() const ;
				void Unbind() const ;
				void GenerateMipMaps() const;

				void AddTexture(const std::string& texture_name);

				inline void set_width(const GLuint width) {
					width_ = width;
				}

				inline void set_height(const GLuint height) {
					height_ = height;
				}

				inline void set_dimensions(const GLuint width, const GLuint height) {
					set_width(width);
					set_height(height);
				}

				inline void set_max_textures(const GLuint max_textures) {
					max_texture_units_ = max_textures;
				}

				inline void set_texture_array_id(const GLuint slot) {
					textureArrayID_ = slot;
				}

				inline void set_attr(const GLuint width, const GLuint height, const GLuint max_textures, const GLuint slot = 0) {
					set_dimensions(width, height);
					set_max_textures(max_textures);
					set_texture_array_id(slot);
				}

				inline GLuint get_width() const { 
					return width_; 
				}
				inline GLuint get_height() const { 
					return height_; 
				}
				inline GLuint get_maxTextures() const { 
					return max_texture_units_; 
				}
				inline GLuint get_textureArrayID() const { 
					return textureArrayID_; 
				}
			private:

				GLuint max_texture_units_ = 0;
				GLuint textureArrayID_ = 0;
				GLuint width_ = 0;
				GLuint height_ = 0;
				std::list<std::string> textures_ = {};
			};

		}
	}
}