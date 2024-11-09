#include "engine_shaderProgram.h"

namespace Engine {
    namespace Graphics {

        // Constructor
        Shader::Shader() {
            ID_ = GL::create_program();
            if (ID_ == 0) {
                std::cerr << "ERROR::SHADER::Failed to create shader program." << std::endl;
            }
        }

        // Destructor
        Shader::~Shader() {
            cleanup();
        }

        // Add a shader from a file
        void Shader::add_shader(GLenum shaderType, const std::string& shaderFile) {
            // Create the shader object
            GLuint shader = GL::create_shader(shaderType);
            if (shader == 0) {
                std::cerr << "ERROR::SHADER::Failed to create shader of type " << shaderType << "." << std::endl;
                throw std::runtime_error("Shader creation failed");
            }

            // Read the shader source code from the file
            std::string shaderCode = get_file_contents(shaderFile);
            const char* shaderSource = shaderCode.c_str();

            // Compile the shader
            GL::shader_source(shader, 1, &shaderSource, nullptr);
            GL::compile_shader(shader);

            // Check for compilation errors
            GLint success;
            GL::get_shader_iv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                GL::get_shader_info_log(shader, sizeof(infoLog), nullptr, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << shaderType << "\n"
                    << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                GL::delete_shader(shader); // Avoid shader resource leak
                throw std::runtime_error("Shader compilation failed");
            }

            // Attach the shader to the program
            GL::attach_shader(ID_, shader);
            shaders_.push_back(shader); // Store for deletion after linking
        }

        // Link and validate the shader program
        void Shader::link_program() {
            // Link the shader program
            GL::link_program(ID_);

            // Check for linking errors
            GLint success;
            GL::get_program_iv(ID_, GL_LINK_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                GL::get_program_info_log(ID_, sizeof(infoLog), nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog
                    << "\n -- --------------------------------------------------- -- " << std::endl;
                throw std::runtime_error("Program linking failed");
            }

            // Validate the shader program
            GL::validate_program(ID_);
            GL::get_program_iv(ID_, GL_VALIDATE_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                GL::get_program_info_log(ID_, sizeof(infoLog), nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_VALIDATION_ERROR\n" << infoLog
                    << "\n -- --------------------------------------------------- -- " << std::endl;
                throw std::runtime_error("Program validation failed");
            }

            // Delete the shader objects now that they've been linked
            for (auto shader : shaders_) {
                GL::delete_shader(shader);
            }
            shaders_.clear();
        }

        // Activate the shader program
        void Shader::activate() const {
            GL::use_program(ID_);
        }

        // Delete the shader program
        void Shader::cleanup() {
            if (ID_ != 0) {
                GL::delete_program(ID_);
                ID_ = 0;
            }
        }

        // Get the shader program ID
        [[nodiscard]] GLuint Shader::get_ID() const noexcept {
            return ID_;
        }

        // Equality operator
        bool Shader::operator==(const Shader& other) const noexcept {
            return ID_ == other.ID_;
        }

        // Inequality operator
        bool Shader::operator!=(const Shader& other) const noexcept {
            return !(*this == other);
        }

        // Read file contents
        std::string Shader::get_file_contents(const std::string& filename) const {
            // Define the shader path (adjust as needed)
            std::string shader_path = "GemEngine/resources/shaders/";
            std::string full_filename = shader_path + filename;

            std::ifstream in(full_filename, std::ios::in | std::ios::binary);
            if (in) {
                std::ostringstream contents;
                contents << in.rdbuf();
                in.close();
                return contents.str();
            }
            std::cout << full_filename << '\n';
            throw std::runtime_error("Could not open file " + full_filename);
        }

    } // namespace Graphics
} // namespace Engine
