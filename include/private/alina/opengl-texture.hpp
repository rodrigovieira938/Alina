#pragma once
#include <alina/alina.hpp>
namespace alina::opengl {
    class Texture : public ITexture {
    public:
        Texture(const TextureDesc& desc, IDevice* device);
        TextureFormat getFormat() override;

        uint32_t id;
        TextureDesc desc;
    };
}