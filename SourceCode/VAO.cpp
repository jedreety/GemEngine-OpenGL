#include"VAO.h"

namespace Engine
{

	namespace GL
	{

		VAO::VAO() {
			nb_object_ = 1;
		}

		VAO::~VAO() {
			cleanup();
		}

		void VAO::cleanup() {
			if (is_generated_) {
				glDeleteVertexArrays(nb_object_, &ID_);
				ID_ = 0;
				nb_object_ = 0;
				is_generated_ = false;
			}

		}

		void VAO::generate() {
			if (!is_generated_) {
				glGenVertexArrays(nb_object_, &ID_);
				is_generated_ = true;
			}	
		}

		void VAO::bind() const {
			if (is_generated_) {
				glBindVertexArray(ID_);
			}
			else {
				std::cerr << "VAO not generated before binding." << std::endl;
			}
			
		}

		void VAO::unbind() {
			glBindVertexArray(0);
		}

		void VAO::link_attrib(Buffer& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
			VBO.bind();
			glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
			glEnableVertexAttribArray(layout);
		}

	}
}