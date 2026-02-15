#include <GL/glew.h>        
#include <GLFW/glfw3.h>   
#include <glm/glm.hpp>  
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "Buffer.h"
#include "BufferLayout.h"
#include "GLutils.h"
#include "ParticleSystem.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"

constexpr int WINDOW_WIDTH{960};
constexpr int WINDOW_HEIGHT{960};
constexpr float dt{0.01};

int main()
{

    /****************** window intialisation etc etc  *************************/
    
    // declare the window object
    GLFWwindow* window;

    // initilise
    if (!glfwInit()) return -1;

    // create the window and terminate if error occurs
    // first two arguments control window size in pixels
    window = glfwCreateWindow(WINDOW_HEIGHT, WINDOW_WIDTH, "Particle Simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(2);

    if(glewInit() != GLEW_OK) std::cout << "Error initialising GLEW" << std::endl;

    /******************* done with setting up window stuff *********************/

    
    // create particles
    constexpr int no_particles{2};
    System system(no_particles);
    system.add_body(1.989e30f, 6.957e8f);
    system.add_body(5.97e24f, 6.371e6f, {constants::AU, 0}, {constants::AU, -300});

    // initialise buffers and vao
    Buffer circle_vbo(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    std::vector<Vertex> circle_vertices{generate_circle_vertices(100, WINDOW_WIDTH, WINDOW_HEIGHT)};
    circle_vbo.add_data(static_cast<GLsizeiptr>(circle_vertices.size()*sizeof(Vertex)), circle_vertices.data());

    VertexBufferLayout circle_layout;
    circle_layout.push(GL_FLOAT, 2, GL_FALSE);

    Buffer instance_vbo(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
    GLsizeiptr size{no_particles*sizeof(Position)};
    std::vector<float> data{-0.5f, 0.0f, 0.5f, 0.0f};
    instance_vbo.add_data(size, data.data());

    VertexBufferLayout instance_layout;
    instance_layout.push(GL_FLOAT, 2, GL_FALSE);

    VertexArray vao;
    vao.add_buffer(circle_vbo, circle_layout, {0});
    vao.add_buffer(instance_vbo, instance_layout, {1});
    GLCall(glVertexAttribDivisor(1, 1));

    // shader program
    GLuint shader_id{create_program("res/shaders/SolarSystem.shader")};
    GLCall(glUseProgram(shader_id));

    GLCall(GLint projection_loc{glGetUniformLocation(shader_id, "projection")});
    const glm::mat4 projection{1.0f};
    GLCall(glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection)));

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glDrawArraysInstanced(GL_TRIANGLE_FAN,  0, 102, no_particles));
        // system.update_system(dt);
        // instance_vbo.add_data(size, system.get_positions().data());

        glfwSwapBuffers(window);
        glfwPollEvents();
    };
    glfwTerminate();

    return 0;
}
