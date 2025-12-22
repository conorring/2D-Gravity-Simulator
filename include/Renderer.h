#ifndef RENDERER_H
#define RENDERER_H

#include "Body.h"

#include <GL/glew.h>

#include <csignal>
#include <vector>

namespace constants
{
    constexpr double PI{3.14159265358979323846};
    constexpr double EARTH_ORBIT{1.5e11};           // 1 AU in metres
    constexpr int RES{100};                         // number of triangles to use when drawing bodies
}

/*******************************************************************************************************/
/* open gl error helpers */
/*******************************************************************************************************/
#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


/*******************************************************************************************************/
/* open gl drawing and state manipulation and vertex creation */
/*******************************************************************************************************/

// holds data for an individual vertex
// for now only position
struct Vertex{
    float x;
    float y;
};

// generates a vector of vertices that will be used for drawing a circular object
std::vector<Vertex> generateVertices(const int& res, const float& radius);

// creates vao and returns the vao handle
// vao has instructions to draw from a buffer with data for a circle of given radius centres at (0, 0)
// when drawing occurs a uniform matrix will be used to modify position
GLuint create_body_vao(const float&);

#endif
