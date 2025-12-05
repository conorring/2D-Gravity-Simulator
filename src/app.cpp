#include <GL/glew.h>        // access function pointers
#include <GLFW/glfw3.h>     // manage windowing env

#include "Renderer.h"

#include <iostream>
#include <vector>
#include <cmath>

const double PI{3.14159265358979323846};

struct Vertex{
    float x;
    float y;
};

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
        x1 = radius*cos(2*PI*k / res) + x;
        y1 = radius*sin(2*PI*k / res) + y; 
        vertices.push_back({x1, y1}); 
    }

    return vertices;
}

int main()
{

    // declare the window object
    GLFWwindow* window;

    // initilise
    if (!glfwInit()) return -1;

    // create the window and terminate if error occurs
    window = glfwCreateWindow(640, 480, "Gravity", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(2);

    if(glewInit() != GLEW_OK) std::cout << "Error initialising GLEW" << std::endl;

    /*** defining the vertex coordinates for drawing the circle ***/
    int res{100}; // use 100 triangles to draw circle
    float radius{0.5};
    float centre_x{0.0f};
    float centre_y{0.0f};

    std::vector<Vertex> vertices{generateVertices(res, radius, centre_x, centre_y)};

    // generate and bind the vao
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /*** creating the vertex buffer object ***/
    GLuint vbo;     // will store the vertex buffer id
    GLCall(glGenBuffers(1, &vbo));      // generate the buffer and write the id to vbo (&vbo is the address of vbo)
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo)); // binds the buffer to the vertex target buffer
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0));

    /***  create program and attach and compile shaders ***/

    // create program and shader object
    GLuint program{glCreateProgram()};
    GLuint vshader{glCreateShader(GL_VERTEX_SHADER)};

    // define shader source code
    const std::string vertexShaderSource = R"glsl(
    #version 330 core
    
    layout(location=0) in vec4 position;
    
    void main(){
        gl_Position = position;
    }
    )glsl";

    // convert source code to c style string so we can easily pass to glShaderSource
    const char* src{vertexShaderSource.c_str()};

    // place the source code in the shader object
    GLCall(glShaderSource(vshader, 1, &src, nullptr));

    // compile the source code in the shader with id vshader
    GLCall(glCompileShader(vshader));

    // writes the compile status of the source code to result
    // used for catching compilation errors
    int result;
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &result);

    // if compilation fails write the error log message
    if (result == GL_FALSE)
    {
        // get the lenght of the log message to define message and store it there
        int length;
        glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &length);

        // alloca is useful for allocating memory for the char array when the length is determined at runtime
        char* message = (char*)alloca(length * sizeof(char));

        // writes the log to message
        glGetShaderInfoLog(vshader, length, &length, message);
        std::cout << "Failed to compile vertex shader";
        std::cout << message << std::endl;

        // deletes the shader
        glDeleteShader(vshader); 
    }

    GLCall(glAttachShader(program, vshader));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    glDeleteShader(vshader);

    GLCall(glUseProgram(program));

    while (!glfwWindowShouldClose(window))
    {
        GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, res+2));
        glfwSwapBuffers(window);
        glfwPollEvents();
    };
    glfwTerminate();

    return 0;
}
