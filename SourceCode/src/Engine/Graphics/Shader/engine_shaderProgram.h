#pragma once

#include <GlfwGlad.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace Engine {
    namespace Graphics {

        /**
         * @brief Shader class for managing OpenGL shader programs.
         *
         * The Shader class encapsulates the creation, compilation, linking, and usage of OpenGL shader programs.
         * It supports adding multiple shaders of different types, linking them into a program, and activating the program.
         */
        class Shader {
        public:
            /**
             * @brief Constructs a Shader object.
             *
             * Initializes the shader program.
             */
            Shader();

            /**
             * @brief Destructor that cleans up the shader program.
             *
             * Ensures that the shader program is properly deleted when the Shader instance goes out of scope.
             */
            ~Shader();

            /**
             * @brief Adds a shader of a specified type from a file.
             *
             * Compiles the shader source code from the provided file and attaches it to the shader program.
             *
             * @param shaderType The type of shader (e.g., GL_VERTEX_SHADER).
             * @param shaderFile The path to the shader source file.
             */
            void add_shader(GLenum shaderType, const std::string& shaderFile);

            /**
             * @brief Links and validates the shader program.
             *
             * Links all attached shader objects into a shader program and validates it.
             * Must be called after all shaders have been added.
             */
            void link_program();

            /**
             * @brief Activates the shader program.
             *
             * Makes the shader program the current program object.
             */
            void activate() const;

            /**
             * @brief Deletes the shader program.
             *
             * Deletes the shader program and resets the program ID.
             */
            void cleanup();

            /**
             * @brief Gets the shader program ID.
             *
             * @return The OpenGL shader program ID.
             */
            [[nodiscard]] GLuint get_ID() const noexcept;

            /**
             * @brief Equality operator.
             *
             * Compares two Shader objects based on their program IDs.
             *
             * @param other The other Shader object to compare with.
             * @return True if both shaders have the same program ID, false otherwise.
             */
            bool operator==(const Shader& other) const noexcept;

            /**
             * @brief Inequality operator.
             *
             * Compares two Shader objects based on their program IDs.
             *
             * @param other The other Shader object to compare with.
             * @return True if the shaders have different program IDs, false otherwise.
             */
            bool operator!=(const Shader& other) const noexcept;

        private:
            GLuint ID_ = 0;                     ///< OpenGL shader program ID.
            std::vector<GLuint> shaders_;       ///< Container for shader object IDs.

            /**
             * @brief Reads the contents of a file into a string.
             *
             * @param filename The path to the file.
             * @return The contents of the file as a string.
             */
            std::string get_file_contents(const std::string& filename) const;
        };

    } // namespace Graphics
} // namespace Engine
