#pragma once

#include "alina/opengl-device.hpp"
#include <alina/alina.hpp>
namespace alina::opengl {
    class GlSampler : public ISampler {
    public:
        GlSampler(const SamplerDesc& desc, GlDevice* device);
        IDevice* getDevice() override {return device;}
        std::string getName() override;
        void setName(const std::string& name) override;
        GlDevice* device;
        uint32_t id;
    };
};