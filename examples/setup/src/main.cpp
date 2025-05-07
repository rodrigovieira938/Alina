#include "alina/opengl.hpp"
#include <GLFW/glfw3.h>
#include <alina/alina.hpp>

void test();

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Alina Setup Example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    auto device = alina::opengl::CreateDevice(glfwGetProcAddress);
    auto buffer = device->createBuffer(
        alina::BufferDesc()
            .setDebugName("VertexBuffer1")
            .setType(alina::BufferType::VERTEX)
    );
    

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