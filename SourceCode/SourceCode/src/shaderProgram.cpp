#include "shaderProgram.h"

namespace Engine
{
    namespace Graphics
    {
        Shader::Shader() {
            ID_ = glCreateProgram();
        }

        Shader::~Shader() {
            Delete();
        }

        void Shader::add_shader(GLenum shaderType, const char* shaderFile)  {

            GLuint shader = glCreateShader(shaderType);

            std::string shaderCode = get_file_contents(shaderFile);
            const char* shaderSource = shaderCode.c_str();

            // Compile the shader
            glShaderSource(shader, 1, &shaderSource, nullptr);
            glCompileShader(shader);

            // Check for compilation errors
            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << shaderType << "\n"
                    << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                glDeleteShader(shader); // Avoid shader resource leak
                throw std::runtime_error("Shader compilation failed");
            }

            // Attach the shader to the program
            glAttachShader(ID_, shader);
            shaders_.push_back(shader); // Store for deletion after linking
        }

        void Shader::link_program() {
            // Link the shader program
            glLinkProgram(ID_);

            // Check for linking errors
            GLint success;
            glGetProgramiv(ID_, GL_LINK_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                glGetProgramInfoLog(ID_, sizeof(infoLog), nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog
                    << "\n -- --------------------------------------------------- -- " << std::endl;
                throw std::runtime_error("Program linking failed");
            }

            // Validate the shader program
            glValidateProgram(ID_);
            glGetProgramiv(ID_, GL_VALIDATE_STATUS, &success);
            if (!success) {

                char infoLog[1024];
                glGetProgramInfoLog(ID_, sizeof(infoLog), nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_VALIDATION_ERROR\n" << infoLog
                    << "\n -- --------------------------------------------------- -- " << std::endl;
                throw std::runtime_error("Program validation failed");
            }

            // Delete the shader objects now that they've been linked
            for (auto shader : shaders_) {
                glDeleteShader(shader);
            }
            shaders_.clear();
        }

        void Shader::Activate() const {
            glUseProgram(ID_);
        }

        void Shader::Delete() {
            if (ID_ != 0)
            {
                glDeleteProgram(ID_);
                ID_ = 0;
            }
        }

        std::string Shader::get_file_contents(const std::string& filename) const {

			std::string shader_path = "SourceCode\\Ressources\\shader\\";
            std::string full_filename = shader_path + filename;

            std::ifstream in(full_filename, std::ios::in | std::ios::binary);
            
            if (in)
            {
                std::ostringstream contents;
                contents << in.rdbuf();
                in.close();
                return contents.str();
            }
            throw std::runtime_error(std::string("Could not open file ") + full_filename);
        }
    }
}
