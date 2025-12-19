#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

#include <csignal>
#include <vector>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


// clears the error stack for gl calls
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

constexpr double PI{3.14159265358979323846};

struct Vertex{
    float x;
    float y;
};

std::vector<Vertex> generateVertices(int, float, float, float);

#endif
