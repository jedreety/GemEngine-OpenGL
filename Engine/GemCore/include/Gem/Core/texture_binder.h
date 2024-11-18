#pragma once

#include <GlfwGlad.h>
#include <vector>
#include <Gem/Graphics/textures/texture.h>

namespace Gem {

    namespace Core {

        /**
         * @brief TextureBinder class for managing the binding of multiple textures.
         *
         * The TextureBinder class allows you to bind multiple textures to specified texture units,
         * simplifying the process of setting up textures for rendering.
         */
        class TextureBinder {

        public:
            /**
             * @brief Constructs a TextureBinder object.
             */
            TextureBinder();

            /**
             * @brief Destructor that unbinds all textures.
             */
            ~TextureBinder();

            /**
             * @brief Binds a texture to a specified texture unit.
             *
             * @param texture Pointer to the Texture object.
             * @param texture_unit The texture unit index (e.g., 0 for GL_TEXTURE0).
             */
            void bind_texture(const Gem::Graphics::Texture* texture, GLuint texture_unit);

            /**
             * @brief Unbinds all bound textures.
             */
            void unbind_all();

        private:
            std::vector<const Gem::Graphics::Texture*> bound_textures_;
        };

    } // namespace Graphics

} // namespace Gem
