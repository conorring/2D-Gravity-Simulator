#include "Renderer.h"

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
std::vector<Vertex> generateVertices(int res, float radius, float x, float y)
{
    std::vector<Vertex> vertices;
    vertices.push_back({x, y}); // add centre of circle
    
    int k;
    float x1, y1;

    // less than or equal to include first point again for full circle
    for(k=0; k<res+1; k++)
    {
        x1 = radius*std::cos(2*PI*k / res) + x;
        y1 = radius*std::sin(2*PI*k / res) + y; 
        vertices.push_back({x1, y1}); 
    }

    return vertices;
}



