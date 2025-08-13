#pragma once

#include "alina/alina.hpp"
#include <alina/opengl-device.hpp>
#include <stdint.h>

namespace alina::opengl {
    class GlBuffer : public IBuffer {
    public:
        GlBuffer(const BufferDesc& desc, GlDevice* device);
        BufferType getType() const override {return mType;}

        GlDevice* mDevice;
        BufferType mType;
        uint32_t mID;
        size_t size = 0;
    };
}