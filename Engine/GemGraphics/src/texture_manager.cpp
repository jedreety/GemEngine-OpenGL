#include <Gem/Graphics/texture_manager.h>

namespace Gem {

    namespace Graphics {

            // Constructor
            Texture2DArray::Texture2DArray() {
                // Default attributes are already set in the member initializer list.
            }

            // Destructor
            Texture2DArray::~Texture2DArray() {
                if (texture_array_ID_ != 0) {
                    GL::delete_textures(1, &texture_array_ID_);
                    texture_array_ID_ = 0;
                }
            }

            // Initialize the texture array
            void Texture2DArray::init() {
                // Check if the texture attributes are set
                if (!is_attr_set()) {
                    std::cerr << "ERROR::Texture2DArray::init: Initialize texture manager dimensions and max texture units first." << std::endl;
                    throw std::runtime_error("Texture2DArray attributes not set.");
                }

                generate();

                bind();

                // Allocate storage for the texture array

                GL::tex_storage_3d(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width_, height_, max_texture_units_);

                unbind();

                is_initialized_ = true;
            }

            // Generate the texture array
            void Texture2DArray::generate() {
                GL::gen_textures(1, &texture_array_ID_);
                if (texture_array_ID_ == 0) {
                    std::cerr << "ERROR::Texture2DArray::generate: Failed to generate texture array." << std::endl;
                    throw std::runtime_error("Failed to generate texture array.");
                }
            }

            // Check if attributes are set
            [[nodiscard]] bool Texture2DArray::is_attr_set() const noexcept {
                return width_ > 0 && height_ > 0 && max_texture_units_ > 0;
            }

            // Bind the texture array
            void Texture2DArray::bind() const {
                GL::active_texture(GL_TEXTURE0 + texture_unit_);
                GL::bind_texture(GL_TEXTURE_2D_ARRAY, texture_array_ID_);
            }

            // Unbind the texture array
            void Texture2DArray::unbind() const {
                GL::bind_texture(GL_TEXTURE_2D_ARRAY, 0);
            }

            // Generate mipmaps
            void Texture2DArray::generate_mipmaps() const {
                if (!is_initialized_) {
                    std::cerr << "ERROR::Texture2DArray::generate_mipmaps: Texture array not initialized." << std::endl;
                    throw std::runtime_error("Texture array not initialized.");
                }
                bind();
                GL::generate_mipmap(GL_TEXTURE_2D_ARRAY);
                unbind();
            }

            // Add a texture to the array
            void Texture2DArray::add_texture(const std::string& texture_name) {
                if (!is_initialized_) {
                    std::cerr << "ERROR::Texture2DArray::add_texture: Texture array not initialized. Call init() first." << std::endl;
                    throw std::runtime_error("Texture array not initialized.");
                }

                if (textures_.size() >= max_texture_units_) {
                    std::cerr << "ERROR::Texture2DArray::add_texture: Maximum number of textures reached." << std::endl;
                    return;
                }

                if (std::find(textures_.begin(), textures_.end(), texture_name) != textures_.end()) {
                    std::cerr << "ERROR::Texture2DArray::add_texture: Texture '" << texture_name << "' already exists." << std::endl;
                    return;
                }

                // Load the texture image
                int width, height, channels;
                std::string full_filename = path_ + texture_name;

                stbi_set_flip_vertically_on_load(true); // Flip the image vertically if needed
                unsigned char* texture_data = stbi_load(full_filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

                if (!texture_data) {
                    std::cerr << "ERROR::Texture2DArray::add_texture: Failed to load texture '" << full_filename << "',\ntry to change the path with set_path() to your local texture folder." << std::endl;
                    return;
                }

                if (static_cast<GLuint>(width) != width_ || static_cast<GLuint>(height) != height_) {
                    std::cerr << "ERROR::Texture2DArray::add_texture: Texture '" << full_filename << "' has different dimensions than "<< width_ << 'x' << height_ << '.' << std::endl;
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
            void Texture2DArray::set_dimensions(GLuint width, GLuint height) {
                width_ = width;
                height_ = height;
            }

            // Set maximum number of textures
            void Texture2DArray::set_max_textures(GLuint max_textures) {
                max_texture_units_ = max_textures;
            }

            // Set texture unit
            void Texture2DArray::set_texture_unit(GLuint texture_unit) {
                texture_unit_ = texture_unit;
            }

            // Set the min filter parameter
            void Texture2DArray::set_min_filter(GLint param) {

                bind();

                GL::tex_parameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, param);
                unbind();
            }

            // Set the mag filter parameter
            void Texture2DArray::set_mag_filter(GLint param) {

                bind();

                GL::tex_parameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, param);
            }

            // Set the wrap parameter
            void Texture2DArray::set_wrap(GLint param) {

                bind();

                GL::tex_parameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, param);
                GL::tex_parameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, param);
                unbind();
            }

            // Set the wrap s filter parameter
            void Texture2DArray::set_wrap_s(GLint param) {

                bind();

                GL::tex_parameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, param);
                unbind();
            }

            // Set the wrap t filter parameter
            void Texture2DArray::set_wrap_t(GLint param) {

                bind();

                GL::tex_parameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, param);
                unbind();
            }
            
            // Set the path to the texture folder
            void Texture2DArray::set_path(const std::string& path) {
                path_ = path;
            }

            // Getters
            [[nodiscard]] GLuint Texture2DArray::get_width() const noexcept {
                return width_;
            }

            [[nodiscard]] GLuint Texture2DArray::get_height() const noexcept {
                return height_;
            }

            [[nodiscard]] GLuint Texture2DArray::get_max_textures() const noexcept {
                return max_texture_units_;
            }

            [[nodiscard]] GLuint Texture2DArray::get_texture_array_ID() const noexcept {
                return texture_array_ID_;
            }

            // Equality operator
            bool Texture2DArray::operator==(const Texture2DArray& other) const noexcept {
                return texture_array_ID_ == other.texture_array_ID_;
            }

            // Inequality operator
            bool Texture2DArray::operator!=(const Texture2DArray& other) const noexcept {
                return !(*this == other);
            }

    } // namespace Graphics

} // namespace Gem
