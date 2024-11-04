#pragma once

#include <glad/glad.h>
#include <cstdint>   // for fixed-width integer types
#include <vector>
#include <string>
#include <algorithm> // for std::find
#include <iostream>
#include <stb/stb_image.h>

namespace Engine {
    namespace Graphics {
        namespace Texture {

            /**
             * @brief TextureManager class for managing a texture array in OpenGL.
             *
             * The TextureManager class encapsulates the creation, binding, and management of a texture array.
             * It allows loading multiple textures into a single texture array for efficient rendering.
             */
            class TextureManager {
            public:
                /**
                 * @brief Constructs a TextureManager object with default attributes.
                 *
                 * Initializes default values for texture width, height, and maximum texture units.
                 */
                TextureManager();

                /**
                 * @brief Destructor that cleans up the texture array.
                 *
                 * Ensures that the texture array is properly deleted when the TextureManager instance goes out of scope.
                 */
                ~TextureManager();

                /**
                 * @brief Initializes the texture array.
                 *
                 * Generates the texture array and sets texture parameters.
                 * Must be called after setting desired attributes and before adding textures.
                 */
                void init();

                /**
                 * @brief Binds the texture array to the active texture unit.
                 */
                void bind() const;

                /**
                 * @brief Unbinds the texture array.
                 */
                void unbind() const;

                /**
                 * @brief Generates mipmaps for the texture array.
                 */
                void generate_mipmaps() const;

                /**
                 * @brief Adds a texture to the texture array.
                 *
                 * Loads a texture from a file and adds it to the texture array.
                 * The texture must have the same dimensions as specified in the texture manager.
                 *
                 * @param texture_name The name of the texture file (without extension).
                 */
                void add_texture(const std::string& texture_name);

                /**
                 * @brief Sets the dimensions of the textures in the array.
                 *
                 * @param width The width of the textures.
                 * @param height The height of the textures.
                 */
                void set_dimensions(GLuint width, GLuint height);

                /**
                 * @brief Sets the maximum number of textures in the array.
                 *
                 * @param max_textures The maximum number of textures.
                 */
                void set_max_textures(GLuint max_textures);

                /**
                 * @brief Sets the texture unit to bind the texture array to.
                 *
                 * @param texture_unit The texture unit index (e.g., 0 for GL_TEXTURE0).
                 */
                void set_texture_unit(GLuint texture_unit);

                /**
                 * @brief Gets the width of the textures.
                 *
                 * @return The texture width.
                 */
                [[nodiscard]] GLuint get_width() const noexcept;

                /**
                 * @brief Gets the height of the textures.
                 *
                 * @return The texture height.
                 */
                [[nodiscard]] GLuint get_height() const noexcept;

                /**
                 * @brief Gets the maximum number of textures.
                 *
                 * @return The maximum number of textures.
                 */
                [[nodiscard]] GLuint get_max_textures() const noexcept;

                /**
                 * @brief Gets the texture array ID.
                 *
                 * @return The OpenGL texture array ID.
                 */
                [[nodiscard]] GLuint get_texture_array_ID() const noexcept;

                /**
                 * @brief Equality operator.
                 *
                 * Compares two TextureManager objects based on their texture array IDs.
                 *
                 * @param other The other TextureManager object to compare with.
                 * @return True if both have the same texture array ID, false otherwise.
                 */
                bool operator==(const TextureManager& other) const noexcept;

                /**
                 * @brief Inequality operator.
                 *
                 * Compares two TextureManager objects based on their texture array IDs.
                 *
                 * @param other The other TextureManager object to compare with.
                 * @return True if they have different texture array IDs, false otherwise.
                 */
                bool operator!=(const TextureManager& other) const noexcept;

            private:
                GLuint texture_array_ID_ = 0;         ///< OpenGL texture array ID.
                GLuint max_texture_units_ = 16;       ///< Maximum number of textures in the array.
                GLuint width_ = 16;                   ///< Width of each texture.
                GLuint height_ = 16;                  ///< Height of each texture.
                GLuint texture_unit_ = 0;             ///< Texture unit index.
                bool is_initialized_ = false;         ///< Flag indicating if the texture array has been initialized.

                std::vector<std::string> textures_;   ///< List of texture names added to the array.

                /**
                 * @brief Checks if the texture attributes are set.
                 *
                 * @return True if attributes are set, false otherwise.
                 */
                [[nodiscard]] bool is_attr_set() const noexcept;

                /**
                 * @brief Generates the texture array.
                 */
                void generate();
            };

        } // namespace Texture
    } // namespace Graphics
} // namespace Engine
