#pragma once

#include<glad/glad.h>
#include "VBO.h"

namespace Engine
{

	namespace GL
	{
		class VAO
		{
		public:

			VAO();

			// generates a VAO ID
			void generate();

			// set the number of object of the VAO
			void set_nb_object(const GLsizei nb_object);

			// Links a VBO to the VAO using a certain layout
			void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

			// Binds the VAO
			void Bind();

			// Unbinds the VAO
			void Unbind();

			// Deletes the VAO
			void Delete() const;

			inline GLuint get_ID() const {
				return ID_;
			};

			inline GLsizei get_nb_object() const {
				return nb_object_;
			};

		private:
			GLuint ID_;
			GLsizei nb_object_;
		};
	}
}