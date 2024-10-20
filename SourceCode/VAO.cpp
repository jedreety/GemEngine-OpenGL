#include"VAO.h"

namespace Engine
{

	namespace GL
	{

		VAO::VAO() {
			ID_ = 0;
			nb_object_ = 1;
		}

		void VAO::gen_VAO() {
			glGenVertexArrays(nb_object_, &ID_);
		}

		void VAO::set_nb_object(const GLsizei nb_object) {
			nb_object_ = nb_object;
		}

		void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
			VBO.Bind();
			glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
			glEnableVertexAttribArray(layout);
			VBO.Unbind();
		}

		void VAO::Bind() {
			glBindVertexArray(ID_);
		}

		void VAO::Unbind() {
			glBindVertexArray(0);
		}

		void VAO::Delete() const {
			glDeleteVertexArrays(1, &ID_);
		}
	}
}