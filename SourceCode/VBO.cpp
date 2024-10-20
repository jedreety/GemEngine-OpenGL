#include"VBO.h"

namespace Engine
{

	namespace GL
	{

		VBO::VBO() {
			ID_ = 0;
			nb_object_ = 1;
		}

		void VBO::gen_VBO() {
			glGenBuffers(nb_object_, &ID_);
		}

		void VBO::set_nb_object(const GLsizei nb_object) {
			nb_object_ = nb_object;
		}

		void VBO::insert_vertices(GLfloat* vertices, GLsizeiptr size) {
			Bind();
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}

		void VBO::Bind() {
			glBindBuffer(GL_ARRAY_BUFFER, ID_);
		}

		void VBO::Unbind() {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void VBO::Delete() const {
			glDeleteBuffers(nb_object_, &ID_);
		}
	}
}