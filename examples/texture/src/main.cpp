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
    
    // Black and white checkerboard
    auto texture1 = device->createTexture(alina::TextureDesc().setWidth(8).setHeight(8));
    struct Color {
        float r,g,b;
    };
    Color texture_data[8][8];
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            bool is_white = (x + y) % 2 == 0;
            texture_data[y][x] = is_white ? Color{1.0f, 1.0f, 1.0f} : Color{0.0f, 0.0f, 0.0f};
        }
    }

    auto cmd = device->createCommandList();
    cmd->begin();
    cmd->writeTexture(texture1, texture_data, alina::TextureFormat::RGB32_Float);
    cmd->end();
    device->execute(cmd);
    
    auto texture2 = device->createTexture(alina::TextureDesc().setWidth(8).setHeight(8));
    auto texture3 = device->createTexture(alina::TextureDesc().setWidth(8).setHeight(8).setMipLevels(1));
    Color baseColor = {0.2f, 0.6f, 0.9f};
    Color altColor = {1.0f - baseColor.r, 1.0f - baseColor.g, 1.0f - baseColor.b};

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            bool is_white = (x + y) % 2 == 0;
            texture_data[y][x] = is_white ? baseColor : altColor;
        }
    }

    cmd->begin();
    cmd->writeTexture(texture2, texture_data, alina::TextureFormat::RGB32_Float);
    cmd->blitTexture(texture2, texture3);
    cmd->generateMipMaps(texture3);
    cmd->end();
    device->execute(cmd);

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