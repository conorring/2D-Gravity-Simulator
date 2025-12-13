#include "Renderer.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


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



/********************************************************************/
/*   functionality for creating a program from shader source code   */
/********************************************************************/


// shader source code to be stored in a file with a .shader extension in res/shaders
// for each shader type source - begin with #shader type and then the glsl code
ShaderSource parse_shader_source(const std::string& filepath)
{
    std::ifstream stream(filepath);

    if (!stream.is_open()) 
    {
        std::cerr << "Failed to open shader file: " << filepath << std::endl;
        return {}; 
    }

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type{ShaderType::NONE};

    while ( getline(stream, line) )
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else 
        {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}


// creates and returns the ID of a shader with the passed source code compiled in it
GLuint compile_shader(GLuint shader_type, const std::string& source)
{
    // glCompileShader requires char** string
    const char* char_source{source.c_str()};

    GLCall(GLuint shader_id{glCreateShader(shader_type)});
    GLCall(glShaderSource(shader_id, 1, &char_source, nullptr));
    GLCall(glCompileShader(shader_id));

    // check for compilation errors
    int result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

    // if compilation fails
    if (result == GL_FALSE)
    {
        // length - number of chars in info log
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        
        // string to store log message
        std::string message;
        // need to resize as we are writing to message through its address
        message.resize(length);

        glGetShaderInfoLog(shader_id, length, &length, &message[0]);
        std::cerr << "Failed to compile shader";
        std::cerr << message << std::endl;
        glDeleteShader(shader_id); 

        return 0;
    }

    return shader_id;
}

// creates a program with the source code in filepath in it
// does not call glUseProgram so must be done outside this function call
GLuint create_program(const std::string& filepath)
{
    GLuint program_id{glCreateProgram()};
    ShaderSource src{parse_shader_source(filepath)};

    // dont attempt to compile and attach shader if no source code provided
    if(!src.vertex_source.empty())
    {
        GLuint vertex_id{compile_shader(GL_VERTEX_SHADER, src.vertex_source)};
        GLCall(glAttachShader(program_id, vertex_id));
        GLCall(glDeleteShader(vertex_id));
    }

    if(!src.fragment_source.empty())
    {
        GLuint fragment_id{compile_shader(GL_VERTEX_SHADER, src.fragment_source)};
        GLCall(glAttachShader(program_id, fragment_id));
        GLCall(glDeleteShader(fragment_id));
    }

    GLCall(glLinkProgram(program_id));

    int result;
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);

    if(result==GL_FALSE)
    {
        // length - number of chars in info log
        int length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);
        
        // string to store log message
        std::string message;
        // need to resize as we are writing to message through its address
        message.resize(length);

        glGetShaderInfoLog(program_id, length, &length, &message[0]);
        std::cerr << "Failed to link program";
        std::cerr << message << std::endl;
        glDeleteProgram(program_id); 

        return 0;
    }

    // dont know if this is needed
    GLCall(glValidateProgram(program_id));

    return program_id;
}
