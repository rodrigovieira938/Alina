#pragma once

#include <alina/alina.hpp>
namespace alina::opengl {
    class Sampler : public ISampler {
    public:
        Sampler(const SamplerDesc& desc, IDevice* device);

        uint32_t id;
    };
};