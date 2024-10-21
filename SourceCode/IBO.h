#pragma once

#include<glad/glad.h>

namespace Engine
{

	namespace GL
	{
		class IBO
		{
		public:

			IBO();
			void generate();
			void set_nb_object(const GLsizei nb_object);
			void insert_indices(GLuint* indices, GLsizeiptr size);

			// Binds the EBO
			void Bind();
			// Unbinds the EBO
			void Unbind();
			// Deletes the EBO
			void Delete () const;

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
