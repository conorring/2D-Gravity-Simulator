#include <GLFW/glfw3.h>
#include <iostream>

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

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    };
    glfwTerminate();

    return 0;
}
