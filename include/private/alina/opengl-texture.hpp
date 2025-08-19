#pragma once
#include "alina/opengl-device.hpp"
#include <alina/alina.hpp>
namespace alina::opengl {
    class GlTexture : public ITexture {
    public:
        GlTexture(const TextureDesc& desc, GlDevice* device);
        GlTexture(uint32_t id, GlDevice* device);
        
        TextureFormat getFormat() override;
        IDevice* getDevice() override {return device;}
        std::string getName() override;
        void setName(const std::string& name) override;

        uint32_t id;
        TextureDesc desc;
        GlDevice* device;
    };
}