#pragma once

#include <alina/alina.hpp>
namespace alina::opengl {
    class Sampler : public ::alina::Sampler {
    public:
        Sampler(const SamplerDesc& desc, ::alina::Device* device);

        uint32_t id;
    };
};