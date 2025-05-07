#include <alina/opengl.hpp>
#include <alina/opengl-device.hpp>

namespace alina::opengl {
    ::alina::Device* CreateDevice(glLoadFunction fn) {
        return new ::alina::opengl::Device(fn);
    }
}