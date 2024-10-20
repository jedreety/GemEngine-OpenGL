
#include "shaderProgram.h"

// Function to read the entire contents of a file and return it as a std::string
std::string get_file_contents(const char* filename)
{
	// Open the file using an input file stream (ifstream) in binary mode
	std::ifstream in(filename, std::ios::binary);

	// Check if the file was successfully opened
	if (in)
	{
		std::string contents; // String to hold the contents of the file

		// Move the file pointer to the end of the file to determine the file size
		in.seekg(0, std::ios::end);

		// Resize the string to hold the entire file content
		contents.resize(in.tellg()); // tellg() returns the current position, which is the file size

		// Move the file pointer back to the beginning of the file
		in.seekg(0, std::ios::beg);

		// Read the entire file content into the string
		in.read(&contents[0], contents.size());

		// Close the file stream
		in.close();

		// Return the contents of the file
		return(contents);
	}

	// If the file couldn't be opened, throw an error with the appropriate error code (errno)
	throw(errno);
}

namespace Engine
{

	namespace Graphics
	{

		Shader::Shader() {
			ID_ = glCreateProgram();
			vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
			fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
		}

		// Deletes the Shader Program
		Shader::~Shader() {
			glDeleteProgram(ID_);
		}

		void Shader::create_shader(const char* vertexFile, const char* fragmentFile) {
			// Read and Compile the vertex file to store it in our variable
			read_compile_shader(vertexShader_, vertexFile);
			// Read and Compile the fragment file to store it in our variable
			read_compile_shader(fragmentShader_, fragmentFile);

			// Attach the Vertex and Fragment Shaders to the Shader Program
			attach_shader();

			// Link the Shader Program
			link_shader();

			// Delete the now unneeded Vertex and Fragment Shader objects
			delete_shader_objects();
		}

		void Shader::set_vertex_shader(const char* vertexFile) {
			// Read and Compile the vertex file to store it in our variable
			read_compile_shader(vertexShader_, vertexFile);

			// Attach the Vertex Shader to the Shader Program
			glAttachShader(ID_, vertexShader_);
		}

		void Shader::set_fragment_shader(const char* fragmentFile) {
			// Read and Compile the fragment file to store it in our variable
			read_compile_shader(fragmentShader_, fragmentFile);

			// Attach the Fragment Shader to the Shader Program
			glAttachShader(ID_, fragmentShader_);
		}

		void Shader::read_compile_shader(GLuint shader, const char* shaderFile) {
			int success;
			char infoLog[512];

			// Read vertexFile and store it as a string
			std::string shaderCode = get_file_contents(shaderFile);
			// Convert the strings into character arrays
			const char* shaderSource = shaderCode.c_str();

			// Attach Vertex Shader source to the Vertex Shader Object
			glShaderSource(shader, 1, &shaderSource, 0);
			// Compile the Vertex Shader into machine code
			glCompileShader(shader);
				
			// Check if the shader compiled successfully
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 512, 0, infoLog);
				std::cerr << "Failed to compile the shader! ERR: " << infoLog << std::endl;
			}

		}

		void Shader::attach_shader() {
			// Attach the Vertex and Fragment Shaders to the Shader Program
			glAttachShader(ID_, vertexShader_);
			glAttachShader(ID_, fragmentShader_);
		}

		void Shader::link_shader() {
			int success;
			char infoLog[512];

			// Wrap-up/Link all the shaders together into the Shader Program
			glLinkProgram(ID_);

			// Check if the shader program linked successfully
			glGetProgramiv(ID_, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(ID_, 512, 0, infoLog);
				std::cerr << "Failed to link the shader program! ERR: " << infoLog << std::endl;
			}
		}

		void Shader::delete_shader_objects() {
			// Delete the shaders objects
			glDeleteShader(vertexShader_);
			glDeleteShader(fragmentShader_);
		}

		// Activates the Shader Program
		void Shader::Activate() {
			glUseProgram(ID_);
		}

		void Shader::Delete() {
			glDeleteProgram(ID_);
		}


	}

}