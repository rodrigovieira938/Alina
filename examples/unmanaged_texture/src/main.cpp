#include "alina/opengl.hpp"
#include <GLFW/glfw3.h>
#include <alina/alina.hpp>
#include <cstring>

uint32_t createTexture(alina::opengl::IGlDevice* device) {
    auto context = device->getContext();
    uint32_t texture;
    constexpr auto width = 80, height = 90;
    context.GenTextures(1, &texture);  
    context.BindTexture(GL_TEXTURE_2D, texture);  
    context.TexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    context.GenerateMipmap(GL_TEXTURE_2D);
    return texture;
}

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
    auto device = std::dynamic_pointer_cast<alina::opengl::IGlDevice>(alina::opengl::CreateDevice(glfwGetProcAddress));
    auto texture = device->createUnmanagedTexture(createTexture(device.get()));

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