#include <GL/glew.h>        // access function pointers
#include <GLFW/glfw3.h>     // manage windowing env

#include "Renderer.h"
#include "Shader.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

int main()
{

    /****************** window intialisation etc etc  *************************/
    
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

    /******************* done with setting up window stuff *********************/

    /*** defining the vertex coordinates for drawing the circle ***/
    int res{100}; // use 100 triangles to draw circle
    float radius{0.05};
    float centre_x{0.0f};
    float centre_y{0.0f};

    std::vector<Vertex> vertices{generateVertices(res, radius, centre_x, centre_y)};
    std::vector<Vertex> vertices_2{generateVertices(100, 0.05, 0.5f, -0.5f)};

    // generate and bind the vao
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vao_2;
    glGenVertexArrays(1, &vao_2);
    // glBindVertexArray(vao_2); done below when reading in info for the second object


    /*** creating the vertex buffer object ***/
    // vao 1
    GLuint vbo;     // will store the vertex buffer id
    GLCall(glGenBuffers(1, &vbo));      // generate the buffer and write the id to vbo (&vbo is the address of vbo)
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo)); // binds the buffer to the vertex target buffer
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0));

    // vao 2
    GLuint vbo_2;     // will store the vertex buffer id
    GLCall(glGenBuffers(1, &vbo_2));      // generate the buffer and write the id to vbo (&vbo is the address of vbo)
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo_2)); // binds the buffer to the vertex target buffer
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices_2.size()*sizeof(Vertex), vertices_2.data(), GL_STATIC_DRAW));

    GLCall(glBindVertexArray(vao_2)); // now need to bind vao number 2
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0));

    GLuint program{create_program("res/shaders/Body.shader")};
    GLCall(glUseProgram(program));

    GLCall(int location = glGetUniformLocation(program, "transformation"));
    ASSERT(location != -1);

    float transformation[4][4] { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };

    
    float* mat_ptr{ transformation[0] };
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, mat_ptr)); // GL_FALSE for column major order

    float delta_y{0.0001f};
    float y_loc{0};

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        transformation[3][1] = y_loc;
        GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, mat_ptr));

        GLCall(glBindVertexArray(vao));
        GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, res+2));

        GLCall(glBindVertexArray(vao_2));
        GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, res+2));


        if(y_loc < -1) delta_y=0;
        y_loc -= delta_y;

        glfwSwapBuffers(window);
        glfwPollEvents();
    };
    glfwTerminate();

    return 0;
}
