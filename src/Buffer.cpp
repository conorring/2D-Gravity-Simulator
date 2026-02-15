#include "Buffer.h"

#include <GL/glew.h>

#include "GLutils.h"

Buffer::Buffer(GLenum target, GLenum usage):m_target(target), m_usage(usage)
{
    GLCall(glGenBuffers(1, &m_bufferID));
}

Buffer::~Buffer() noexcept
{
    GLCall(glDeleteBuffers(1, &m_bufferID));
}

Buffer::Buffer(Buffer&& original) noexcept
:m_target(original.m_target), m_usage(original.m_usage), m_bufferID(original.m_bufferID)
{
    // optional to set these to 0 as they are not responsible for any opengl resources
    // original.m_target = 0;
    // original.m_usage = 0;
    original.m_bufferID = 0;
}

Buffer& Buffer::operator=(Buffer&& original) noexcept
{
    // if we try to move buffer to itself then do nothing (C.65)
    if (this != &original)
    {
        // in the case that the object we are moving into already owns a buffer
        if (m_bufferID != 0) GLCall(glDeleteBuffers(1, &m_bufferID));

        m_target = original.m_target;
        m_usage = original.m_usage;
        m_bufferID = original.m_bufferID;

        original.m_bufferID = 0;
    }

    return *this;
}

void Buffer::add_data(GLsizeiptr size, const GLvoid* data) const
{
    bind();

    //cannot use glNamedBufferData as it is OpenGL 4.5+
    GLCall(glBufferData(m_target, size, data, m_usage));
}

void Buffer::add_subdata(GLintptr offset, GLsizeiptr size, const GLvoid* data) const
{
    bind();
    GLCall(glBufferSubData(m_target, offset, size, data));;
}

void Buffer::bind() const
{
    GLCall(glBindBuffer(m_target, m_bufferID));
}

void Buffer::unbind() const
{
    GLCall(glBindBuffer(m_target, 0));
}

