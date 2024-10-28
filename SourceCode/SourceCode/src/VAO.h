#pragma once

#include<iostream>
#include<glad/glad.h>
#include "buffer.h"

namespace Engine
{

	namespace GL
	{
		class VAO
		{
		public:

			VAO();
			~VAO();

			// generates a VAO ID
			void generate();

			// Links a VBO to the VAO using a certain layout
			void link_attrib(Buffer& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

			// Binds the VAO
			void bind() const;

			// Unbinds the VAO
			void unbind();

			void cleanup();

			// set the number of object of the VAO
			inline void set_nb_object(const GLsizei nb_object) {
				nb_object_ = nb_object;
			};


			inline GLuint get_ID() const {
				return ID_;
			};

			inline GLsizei get_nb_object() const {
				return nb_object_;
			};

		private:
			GLuint ID_ = 0;
			GLsizei nb_object_ = 0;
			bool is_generated_ = false;
		};
	}
}