#pragma once
#include <alina/alina.hpp>
namespace alina::opengl {
    class Texture : public ITexture {
    public:
        Texture(const TextureDesc& desc, IDevice* device);
        Texture(uint32_t id, IDevice* device);
        TextureFormat getFormat() override;

        uint32_t id;
        TextureDesc desc;
    };
}