#include "VertexArray.h"

#include <cstddef>
#include <vector>

#include "Buffer.h"
#include "BufferLayout.h"
#include "GLutils.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_vaoID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_vaoID));
}

void VertexArray::bind()
{
    GLCall(glBindVertexArray(m_vaoID));
}
void VertexArray::unbind()
{
    GLCall(glBindVertexArray(0));
}

void VertexArray::add_buffer(const Buffer& buffer, const VertexBufferLayout& layout, const std::vector<GLuint>& locations)
{
    bind();
    buffer.bind();
    const auto& elements{layout.get_elements()};

    std::size_t offset{0};
    
    std::size_t size{elements.size()};
    for(std::size_t i{0}; i < size; i++)
    {
        const VertexBufferElement& element{elements[i]};
        const GLuint location{locations[i]};

        GLCall(glEnableVertexAttribArray(location));
        GLCall(glVertexAttribPointer(location, element.count, element.type, element.normalized, layout.get_stride(), 
          reinterpret_cast<const void*>(offset)));

        offset += static_cast<std::size_t>(element.count) * get_sizeof_type(element.type);
    }
}
