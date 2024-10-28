// Buffer.cpp
#include "Buffer.h"
#include <iostream>

namespace Engine
{
    namespace GL
    {
        Buffer::Buffer() {
            type_ = GL_ARRAY_BUFFER;
            nb_object_ = 1;
        }

        Buffer::~Buffer() {
            cleanup();
        }

        void Buffer::generate() {
            if (!is_generated_) {
                glGenBuffers(nb_object_, &ID_);
                is_generated_ = true;
            }
        }

        void Buffer::bind() const {
            if (is_generated_) {
                glBindBuffer(type_, ID_);
            }
            else {
                std::cerr << "Buffer not generated before binding." << std::endl;
            }
        }

        void Buffer::unbind() const {
            glBindBuffer(type_, 0);
        }

        void Buffer::set_data(GLsizeiptr size, const void* data, GLenum usage) {
            if (is_generated_) {
                bind();
                glBufferData(type_, size, data, usage);
            }
            else {
                std::cerr << "Buffer not generated before setting data." << std::endl;
            }
        }

        void Buffer::cleanup()
        {
            if (is_generated_)
            {
                glDeleteBuffers(1, &ID_);
                ID_ = 0;
                nb_object_ = 0;
                is_generated_ = false;
            }
        }
    }
}
