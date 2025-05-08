#pragma once

#include "alina/alina.hpp"
#include <alina/opengl-device.hpp>
#include <stdint.h>

namespace alina::opengl {
    class Buffer : public ::alina::Buffer {
    public:
        Buffer(const BufferDesc& desc, Device* device);
        BufferType getType() const override {return mType;}

        Device* mDevice;
        BufferType mType;
        uint32_t mID;
        size_t size = 0;
    };
}