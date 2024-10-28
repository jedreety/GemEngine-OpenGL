#include "textureManager.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Texture {


			TextureManager::TextureManager() {
				max_texture_units_ = 0;
				textureArrayID_ = 0;
				width_ = 0;
				height_ = 0;
				textures_ = {};
			}

			void TextureManager::Init() {
				// Check if the windows attributes are set
				if (!is_attr_set()) {
					std::cerr << "ERROR::TextureManager::INIT : Initialise texture manager width heigth and max texture units first." << std::endl;
					exit(EXIT_FAILURE);
					return;
				}

				generate();

				Bind();

				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width_, height_, max_texture_units_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			}

			void TextureManager::generate() {
				glGenTextures(1, &textureArrayID_);
			}

			inline bool TextureManager::is_attr_set() const {
				return width_ && height_ && max_texture_units_;
			}

			void TextureManager::Bind() const {
				glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID_);
			}

			TextureManager::~TextureManager() {
				glDeleteTextures(1, &textureArrayID_);
			}


			void TextureManager::Unbind() const {
				glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
			}

			void TextureManager::GenerateMipMaps() const {
				Bind();
				glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
			}

			void TextureManager::AddTexture(const std::string& texture_name) {
				if (textures_.size() < max_texture_units_) {
					if (std::find(textures_.begin(), textures_.end(), texture_name) != textures_.end()) {
						std::cerr << "ERROR::TextureManager::AddTexture: Texture " << texture_name << " already exists" << std::endl;
					}
					else {
						textures_.push_back(texture_name);

						int width, height, bpp;
						const std::string texture_path = texture_name + ".png";

						unsigned char* texture_image = stbi_load(texture_path.c_str(), &width, &height, &bpp, 4);


						if (width_ != width || height_ != height) {
							std::cerr << "ERROR::TextureManager::AddTexture: Texture " << texture_name << " has different dimensions" << std::endl;
							textures_.pop_back();
							if (texture_image) stbi_image_free(texture_image);
							return;
						}

						if (texture_image) {

							Bind();
							glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, textures_.size() - 1, width_, height_, 1, GL_RGBA, GL_UNSIGNED_BYTE, texture_image);

							stbi_image_free(texture_image);
							Unbind();

						}
						else {
							std::cerr << "Failed to load " << texture_name << " image!" << std::endl;
						}
					}
				}

				else {
					std::cerr << "ERROR::TextureManager::AddTexture: Maximum number of textures reached" << std::endl;
				}
			}
		}
	}
}