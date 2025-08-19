#pragma once
#include "alina/alina.hpp"
#include "alina/opengl-device.hpp"

namespace alina::opengl {
    class GlFramebuffer : public IFramebuffer {
    public:
        GlFramebuffer(const FramebufferDesc& desc, GlDevice* device);
        GlFramebuffer(uint32_t id, GlDevice* device);
        IDevice* getDevice() override {return device;}
        std::string getName() override;
        void setName(const std::string& name) override;
        GlDevice* device;
        uint32_t id;
        FramebufferDesc desc;
    };
}