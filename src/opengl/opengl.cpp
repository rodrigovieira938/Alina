#include <alina/opengl.hpp>
#include <alina/opengl-device.hpp>

namespace alina::opengl {
    Device CreateDevice(glLoadFunction fn) {
        return std::make_shared<GlDevice>(fn);
    }
}