#include <GL/glew.h>        // access function pointers
#include <GLFW/glfw3.h>     // manage windowing env

#include "Renderer.h"
#include "Shader.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

//#define DEBUG

constexpr float DT{0.001};

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

    System system;

    // fix at centre
    Body sun(1000, 0.0f);
    Body earth(1, 0.0f, {0.75f, -0.5f}, {0.0f, 1.0f});  
    Body earth2(1, 0.0f, {-0.75f, 0.5f}, {0.0f, -1.0f});

    GLuint sun_vao{create_body_vao(0.1f)};
    GLuint earth_vao{create_body_vao(0.05f)};

    sun.set_vao(sun_vao);
    earth.set_vao(earth_vao);
    earth2.set_vao(earth_vao);

    #ifdef DEBUG
    std::cout << sun.get_vao() << std::endl;
    std::cout << earth.get_vao() << std::endl;
    #endif

    system.add_body(sun);
    system.add_body(earth);
    system.add_body(earth2);

    #ifdef DEBUG
    std::cout << system.particles.size();
    #endif

    GLuint program{create_program("res/shaders/Body.shader")};
    GLCall(glUseProgram(program));

    GLCall(int location = glGetUniformLocation(program, "transformation"));
    ASSERT(location != -1);

    float transformation[4][4] { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };

    
    float* mat_ptr{ transformation[0] };
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, mat_ptr)); // GL_FALSE for column major order

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        system.draw_system(&transformation[0], location);
        system.update_system(DT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    };
    glfwTerminate();

    return 0;
}
