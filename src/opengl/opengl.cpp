#include <alina/opengl.hpp>
#include <alina/opengl-device.hpp>

namespace alina::opengl {
    IDevice* CreateDevice(glLoadFunction fn) {
        return new opengl::Device(fn);
    }
}