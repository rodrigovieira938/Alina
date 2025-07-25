#include "alina/opengl.hpp"
#include <GLFW/glfw3.h>
#include <alina/alina.hpp>
#include <cstring>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Alina Setup Example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /*Creates the device */
    auto device = alina::opengl::CreateDevice(glfwGetProcAddress);
    
    // Your rendering code

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if(device->beginFrame()) {
            device->endFrame();
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}