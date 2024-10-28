// Buffer.h
#pragma once
#include <glad/glad.h>

namespace Engine
{
    namespace GL
    {
        class Buffer
        {
        public:
            Buffer();
            ~Buffer();

            // Generate the buffer
            void generate();

            // Bind the buffer
            void bind() const;

            // Unbind the buffer
            void unbind() const;

            // Upload data to the buffer
            void set_data(GLsizeiptr size, const void* data, GLenum usage);

            // Delete the buffer
            void cleanup();

            inline GLuint get_ID() const { 
                return ID_; 
            }

			inline void set_type(GLenum type) {
				type_ = type;
			}

			inline void set_nb_object(GLsizei nb_object) {
				nb_object_ = nb_object;
			}

			inline void set_attr(GLenum type, GLsizei nb_object) {
				type_ = type;
				nb_object_ = nb_object;
			}

        private:
            GLuint ID_ = 0;
            GLenum type_ = 0;
            GLsizei nb_object_ = 0;
            bool is_generated_ = false;
        };
    }
}
