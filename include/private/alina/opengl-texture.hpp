#pragma once
#include <alina/alina.hpp>
namespace alina::opengl {
    class Texture : public ::alina::Texture {
    public:
        Texture(const TextureDesc& desc, ::alina::Device* device);
        TextureFormat getFormat() override;

        uint32_t id;
        TextureDesc desc;
    };
}