#include"IBO.h"

namespace Engine
{

	namespace GL
	{
		
		IBO::IBO() {
			ID_ = 0;
			nb_object_ = 1;
		}

		void IBO::gen_IBO() {
			glGenBuffers(nb_object_, &ID_);
		}

		void IBO::set_nb_object(const GLsizei nb_object) {
			nb_object_ = nb_object;
		}

		// Constructor that generates a Elements Buffer Object and links it to indices
		void IBO::insert_indices(GLuint* indices, GLsizeiptr size) {
			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
		}

		// Binds the EBO
		void IBO::Bind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID_);
		}

		// Unbinds the EBO
		void IBO::Unbind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		// Deletes the EBO
		void IBO::Delete() const {
			glDeleteBuffers(nb_object_, &ID_);
		}
	}
}