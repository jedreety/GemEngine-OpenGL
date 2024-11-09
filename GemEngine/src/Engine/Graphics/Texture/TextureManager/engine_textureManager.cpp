#include "engine_textureManager.h"

namespace Engine {
    namespace Graphics {
        namespace Texture {

            // Constructor
            TextureManager::TextureManager() {
                // Default attributes are already set in the member initializer list.
            }

            // Destructor
            TextureManager::~TextureManager() {
                if (texture_array_ID_ != 0) {
                    GL::delete_textures(1, &texture_array_ID_);
                    texture_array_ID_ = 0;
                }
            }

            // Initialize the texture array
            void TextureManager::init() {
                // Check if the texture attributes are set
                if (!is_attr_set()) {
                    std::cerr << "ERROR::TextureManager::init: Initialize texture manager dimensions and max texture units first." << std::endl;
                    throw std::runtime_error("TextureManager attributes not set.");
                }

                generate();

                bind();

                // Set texture parameters
                GL::tex_parameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                GL::tex_parameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                GL::tex_parameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
                GL::tex_parameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

                // Allocate storage for the texture array
                GL::tex_storage_3d(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width_, height_, max_texture_units_);

                unbind();

                is_initialized_ = true;
            }

            // Generate the texture array
            void TextureManager::generate() {
                GL::gen_textures(1, &texture_array_ID_);
                if (texture_array_ID_ == 0) {
                    std::cerr << "ERROR::TextureManager::generate: Failed to generate texture array." << std::endl;
                    throw std::runtime_error("Failed to generate texture array.");
                }
            }

            // Check if attributes are set
            [[nodiscard]] bool TextureManager::is_attr_set() const noexcept {
                return width_ > 0 && height_ > 0 && max_texture_units_ > 0;
            }

            // Bind the texture array
            void TextureManager::bind() const {
                GL::active_texture(GL_TEXTURE0 + texture_unit_);
                GL::bind_texture(GL_TEXTURE_2D_ARRAY, texture_array_ID_);
            }

            // Unbind the texture array
            void TextureManager::unbind() const {
                GL::bind_texture(GL_TEXTURE_2D_ARRAY, 0);
            }

            // Generate mipmaps
            void TextureManager::generate_mipmaps() const {
                if (!is_initialized_) {
                    std::cerr << "ERROR::TextureManager::generate_mipmaps: Texture array not initialized." << std::endl;
                    throw std::runtime_error("Texture array not initialized.");
                }
                bind();
                GL::generate_mipmap(GL_TEXTURE_2D_ARRAY);
                unbind();
            }

            // Add a texture to the array
            void TextureManager::add_texture(const std::string& texture_name) {
                if (!is_initialized_) {
                    std::cerr << "ERROR::TextureManager::add_texture: Texture array not initialized. Call init() first." << std::endl;
                    throw std::runtime_error("Texture array not initialized.");
                }

                if (textures_.size() >= max_texture_units_) {
                    std::cerr << "ERROR::TextureManager::add_texture: Maximum number of textures reached." << std::endl;
                    return;
                }

                if (std::find(textures_.begin(), textures_.end(), texture_name) != textures_.end()) {
                    std::cerr << "ERROR::TextureManager::add_texture: Texture '" << texture_name << "' already exists." << std::endl;
                    return;
                }

                // Load the texture image
                int width, height, channels;
                std::string texture_path = "GemEngine/resources/textures/";
                std::string full_filename = texture_path + texture_name + ".png";

                stbi_set_flip_vertically_on_load(true); // Flip the image vertically if needed
                unsigned char* texture_data = stbi_load(full_filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

                if (!texture_data) {
                    std::cerr << "ERROR::TextureManager::add_texture: Failed to load texture '" << full_filename << "'." << std::endl;
                    return;
                }

                if (static_cast<GLuint>(width) != width_ || static_cast<GLuint>(height) != height_) {
                    std::cerr << "ERROR::TextureManager::add_texture: Texture '" << full_filename << "' has different dimensions." << std::endl;
                    stbi_image_free(texture_data);
                    return;
                }

                // Add the texture data to the texture array
                bind();
                GL::tex_sub_image_3d(GL_TEXTURE_2D_ARRAY, 0, 0, 0, static_cast<GLint>(textures_.size()), width_, height_, 1, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
                unbind();

                // Free the loaded texture data
                stbi_image_free(texture_data);

                // Add the texture name to the list
                textures_.push_back(texture_name);
            }

            // Set texture dimensions
            void TextureManager::set_dimensions(GLuint width, GLuint height) {
                width_ = width;
                height_ = height;
            }

            // Set maximum number of textures
            void TextureManager::set_max_textures(GLuint max_textures) {
                max_texture_units_ = max_textures;
            }

            // Set texture unit
            void TextureManager::set_texture_unit(GLuint texture_unit) {
                texture_unit_ = texture_unit;
            }

            // Getters
            [[nodiscard]] GLuint TextureManager::get_width() const noexcept {
                return width_;
            }

            [[nodiscard]] GLuint TextureManager::get_height() const noexcept {
                return height_;
            }

            [[nodiscard]] GLuint TextureManager::get_max_textures() const noexcept {
                return max_texture_units_;
            }

            [[nodiscard]] GLuint TextureManager::get_texture_array_ID() const noexcept {
                return texture_array_ID_;
            }

            // Equality operator
            bool TextureManager::operator==(const TextureManager& other) const noexcept {
                return texture_array_ID_ == other.texture_array_ID_;
            }

            // Inequality operator
            bool TextureManager::operator!=(const TextureManager& other) const noexcept {
                return !(*this == other);
            }

        } // namespace Texture
    } // namespace Graphics
} // namespace Engine
