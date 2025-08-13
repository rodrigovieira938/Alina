#pragma once

#include "alina/opengl-device.hpp"
#include <alina/alina.hpp>
namespace alina::opengl {
    class GlSampler : public ISampler {
    public:
        GlSampler(const SamplerDesc& desc, GlDevice* device);

        uint32_t id;
    };
};