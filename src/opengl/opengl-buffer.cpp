#include <alina/opengl-buffer.hpp>
#include <alina/opengl-conversions.hpp>

namespace alina::opengl {
    GlBuffer::GlBuffer(const BufferDesc& desc, GlDevice* device) {
        mDevice = static_cast<GlDevice*>(device);
        mType = desc.type;
        auto e = bufferTypeToGl(desc.type);
        mDevice->context.CreateBuffers(1, &mID);
        setName(desc.name);
    }
    std::string GlBuffer::getName() {
        GLsizei length;
        mDevice->context.GetObjectLabel(GL_BUFFER, mID, 0, &length, nullptr);
        std::string str;
        str.resize(length, '\0');
        mDevice->context.GetObjectLabel(GL_BUFFER, mID, length+1, &length, str.data());
        return str;
    }
    void GlBuffer::setName(const std::string& name) {
        mDevice->context.ObjectLabel(GL_BUFFER, mID, name.size(), name.data());
    }
}