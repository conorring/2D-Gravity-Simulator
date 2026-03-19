#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glew.h>

#include <vector>

//TODO 
// 1.

class Buffer;
class VertexBufferLayout;

// void glEnableVertexAttribArray(optional arg GLuint vaobj, GLuint index) if no vao is specified currently bound vao is used
// void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizeistride, const GLvoid* pointer)
class VertexArray
{
    private:
        GLuint m_vaoID;

    public:
        VertexArray();
        ~VertexArray();

        // binds the vao and the buffer
        // enables the attributes at specified locations and calls glVertexAttribPointer with data and layout
        void add_buffer(const Buffer&, const VertexBufferLayout&, const std::vector<GLuint>&);
        
        void bind();
        void unbind();
};

#endif
