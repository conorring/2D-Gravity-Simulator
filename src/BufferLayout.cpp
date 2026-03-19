#include "BufferLayout.h"

#include <GL/glew.h>

#include "GLutils.h"

VertexBufferLayout::VertexBufferLayout():m_stride(0)
{
};

void VertexBufferLayout::push(GLenum type, GLint count, GLboolean normalized)
{
    // OpenGL specifies that count can only take values 1,2,3,4
    ASSERT(count > 0 && count < 5);
    
    m_elements.emplace_back(type, count, normalized);

    // implicit conversion here, maybe need to cast?
    m_stride += static_cast<GLsizei>( count * get_sizeof_type(type) );
}
