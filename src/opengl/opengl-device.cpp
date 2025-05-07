#include "glad/gl.h"
#include <alina/opengl-device.hpp>
#include <alina/opengl-buffer.hpp>

namespace alina::opengl {
    Device::Device(glLoadFunction fn) {
        gladLoadGLContext(&context, fn);
    }
    bool Device::beginFrame() {
        context.Clear(GL_COLOR_BUFFER_BIT);
        return true;
    }
    void Device::endFrame() {
        
    }
    ::alina::Buffer* Device::createBuffer(const BufferDesc& desc) {
        return new Buffer(desc, this);
    }
}
