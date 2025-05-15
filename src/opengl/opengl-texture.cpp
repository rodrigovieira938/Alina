#include "alina/alina.hpp"
#include <alina/opengl-texture.hpp>
#include <alina/opengl-device.hpp>
#include <alina/opengl-conversions.hpp>
#include <math.h>

namespace alina::opengl {
    Texture::Texture(const TextureDesc& desc, ::alina::Device* device) : desc(desc) {
        auto context = ((Device*)device)->context;
        context.CreateTextures(desc.depth == 1 ? GL_TEXTURE_2D : GL_TEXTURE_3D, 1, &id);
        uint32_t width = desc.width;
        uint32_t height = desc.height;
        uint32_t depth = desc.depth;
        if(desc.depth == 1) {
            for(int i = 0; i < desc.mipLevels+1; i++) {
                context.TextureStorage2D(id, i+1, textureFormatsToGl(desc.format), width, height);
                width = std::max<uint32_t>(1, width/2);
                height = std::max<uint32_t>(1, height/2);
            }
        } else {
            for(int i = 0; i < desc.mipLevels+1; i++) {
                context.TextureStorage3D(id, i+1, textureFormatsToGl(desc.format), width, height, depth);
                width = std::max<uint32_t>(1, width/2);
                height = std::max<uint32_t>(1, height/2);
                depth = std::max<uint32_t>(1, depth/2);
            }
        }
    }
    TextureFormat Texture::getFormat() {return desc.format;}
}