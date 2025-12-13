#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

#include <csignal>
#include <string>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


// clears the error stack for gl calls
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);



/********************************************************************/
/*   functionality for creating a program from shader source code   */
/********************************************************************/

// used to return multiple strings from parse_shader_source
// for now we only use vertex and fragment shader
struct ShaderSource
{
    const std::string vertex_source;
    const std::string fragment_source;
};

ShaderSource parse_shader_source(const std::string& filepath);
GLuint compile_shader(GLuint shader_type, const std::string& source);
GLuint create_program(const std::string& filepath);

#endif
