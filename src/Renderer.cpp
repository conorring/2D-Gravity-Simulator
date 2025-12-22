#include "Renderer.h"
#include "Body.h"

#include <cmath>
#include <iostream>
#include <vector>


// clears the error stack for gl calls
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGL Error (" << error << ')' << '\n' <<
            "Function call: " << function << '\n' << file << ": " << line << std::endl;
        return false;
    }
    return true;
}

// res is how many triangles we want to use to approximate the circle
std::vector<Vertex> generateVertices(const int& res, const float& radius)
{
    std::vector<Vertex> vertices;
    vertices.push_back({0.0f, 0.0f}); // add centre of circle
    
    int k;
    float x1, y1;

    // less than or equal to include first point again for full circle
    for(k=0; k<res+1; k++)
    {
        x1 = radius*std::cos(2*constants::PI*k / res);
        y1 = radius*std::sin(2*constants::PI*k / res); 
        vertices.push_back({x1, y1}); 
    }

    return vertices;
}

GLuint create_body_vao(const float& radius)
{
    // generates and binds a vertex array object
    GLuint vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    // create buffer object and bind to ARRAY_BUFFER target
    GLuint vbo;
    GLCall(glGenBuffers(1, &vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    // prep vao for drawing
    std::vector<Vertex> vertices{generateVertices(constants::RES, radius)};                                     // generate vertex data
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));     // read data into buffer we have bound to ARRAY target
    GLCall(glEnableVertexAttribArray(0));                                                                       // enable attribute 0 (position in shader source code)
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0));                                // tell gpu how to interpret data in the vbo

    // unbind buffer and vao
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));

    return vao;
}
