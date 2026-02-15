#ifndef BUFFER_H
#define BUFFER_H

#include <GL/glew.h>

// TODO:
// 1.

class Buffer
{
    private:
        GLenum m_target;
        GLenum m_usage;
        GLuint m_bufferID;

    public:

        // initialises a Buffer object with target and usage
        // generates and stores the bufferID
        Buffer(GLenum target, GLenum usage);
        ~Buffer() noexcept;

        Buffer(const Buffer& original) = delete;
        Buffer& operator=(const Buffer& rhs) = delete;

        Buffer(Buffer&& original) noexcept;
        Buffer& operator=(Buffer&& rhs) noexcept;

        // marked as const although this function data stored in the buffer we have a handle for - function does not modify member vars
        void add_data(GLsizeiptr size, const GLvoid* data) const;

        // note that this function requires add_data to be called prior to use and for offset + size < buffer size
        // marked as const although this function data stored in the buffer we have a handle for - function does not modify member vars
        void add_subdata(GLintptr offset, GLsizeiptr size, const GLvoid* data) const;

        void bind() const;
        void unbind() const;

        const GLuint& get_id() const noexcept {return m_bufferID;}
};

#endif
