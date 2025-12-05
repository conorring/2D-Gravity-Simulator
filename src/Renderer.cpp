#include "Renderer.h"
#include <iostream>

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
