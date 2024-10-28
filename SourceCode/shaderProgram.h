#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Engine
{
    namespace Graphics
    {
        class Shader
        {
        public:
            Shader();
            ~Shader();

            // Adds a shader of any type
            void add_shader(GLenum shaderType, const char* shaderFile);

            // Links and validates the shader program
            void link_program();

            // Activates the Shader Program
            void Activate() const;

            // Deletes the Shader Program
            void Delete();

            // Getter for the program ID
            inline GLuint get_ID() const { return ID_; }

        private:
            GLuint ID_;                      // Shader Program ID
            std::vector<GLuint> shaders_;    // Container for shader objects

            // Reads and compiles a shader from a file
            void read_compile_shader(GLuint shader, const char* shaderFile);

            // Reads the contents of a file into a string
            std::string get_file_contents(const char* filename) const;
        };
    }
}
