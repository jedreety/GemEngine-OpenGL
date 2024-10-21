#pragma once

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);
namespace Engine
{

	namespace Graphics
	{

		class Shader
		{
		public:

			Shader();
			~Shader();
				
			// Creates the Shader Program
			void create_shader(const char* vertexFile, const char* fragmentFile);

			// Creates a Vertex Shader from a file
			void set_vertex_shader(const char* vertexFile);
			// Creates a Fragment Shader from a file
			void set_fragment_shader(const char* fragmentFile);
				
			// Read and Compile a shader file into the shader variable
			void read_compile_shader(GLuint shader, const char* shaderFile);
			// Attach the Vertex and Fragment Shaders to the Shader Program
			void attach_shader();
			// Link the Shader Program
			void link_shader();
			// Delete shader objects
			void delete_shader_objects();

			// Activates the Shader Program
			void Activate();

			void Delete();

			inline GLuint get_ID() const { 
				return ID_; 
			}


		private:
			// Reference to the Shader Program, vertex shader and fragment shader
			GLuint ID_;
			GLuint vertexShader_;
			GLuint fragmentShader_;

		};

	}

}
