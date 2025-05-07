#include "glad/gl.h"
#include <alina/opengl-device.hpp>

namespace alina::opengl {
    Device::Device(glLoadFunction fn) {
        gladLoadGLContext(&mContext, fn);
    }
}
