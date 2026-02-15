#ifndef BUFFERLAYOUT_H
#define BUFFERLAYOUT_H

#include <GL/glew.h>

#include <vector>

// TODO
// 1. 

// used to describe a single attribute of a vertex
struct VertexBufferElement
{
    // underlying type of attribute
    GLenum type;

    // number of components for attribute 
    // Eg. Position = (x, y) => count = 2
    GLint count;

    // should data be normalized or converted directly as fixed point values? (docs.gl)
    GLboolean normalized;
};

// describes the layout of data in buffer in terms of VertexBufferElements
class VertexBufferLayout
{
private:
    // each element describes one attribute of vertex
    std::vector<VertexBufferElement> m_elements;

    // byte size of one vertex
    GLsizei m_stride;

public:
    VertexBufferLayout();

    void push(GLenum type, GLint count, GLboolean normalized);

    const std::vector<VertexBufferElement>& get_elements() const noexcept {return m_elements;}
    const GLsizei& get_stride() const noexcept {return m_stride;}
};

#endif
