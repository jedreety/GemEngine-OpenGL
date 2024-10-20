#pragma once
#include<glad/glad.h>

namespace Engine
{

	namespace GL
	{
		class VBO
		{
		public:

			VBO();

			// generates a VBO ID
			void gen_VBO();
			// set the number of object of the VBO
			void set_nb_object(const GLsizei nb_object);
			// Introduces the vertices to the VBO
			void insert_vertices(GLfloat* vertices, GLsizeiptr size);

			// Binds the VBO
			void Bind();
			// Unbinds the VBO
			void Unbind();
			// Deletes the VBO
			void Delete() const;
		
			GLuint get_ID() const {
				return ID_;
			};

			GLsizei get_nb_object() const {
				return nb_object_;
			};

		private:
			GLuint ID_;
			GLsizei nb_object_;
		};
	}
}
