#include <alina/opengl-buffer.hpp>
#include <alina/opengl-conversions.hpp>

namespace alina::opengl {
    Buffer::Buffer(const BufferDesc& desc, Device* device) {
        mDevice = static_cast<Device*>(device);
        mType = desc.type;
        auto e = bufferTypeToGl(desc.type);
        mDevice->context.CreateBuffers(1, &mID);
        mDevice->context.ObjectLabel(GL_BUFFER, mID, desc.name.size(), desc.name.data());
    }
}