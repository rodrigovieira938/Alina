#include "alina/alina.hpp"
#include <alina/opengl-texture.hpp>
#include <alina/opengl-device.hpp>
#include <alina/opengl-conversions.hpp>
#include <math.h>

namespace alina::opengl {
    Texture::Texture(const TextureDesc& desc, ::alina::Device* device) : desc(desc) {
        auto context = ((Device*)device)->context;
        context.CreateTextures(desc.depth == 1 ? GL_TEXTURE_2D : GL_TEXTURE_3D, 1, &id);
        if(desc.depth == 1) {
            context.TextureStorage2D(id, desc.mipLevels+1, textureFormatsToGl(desc.format), desc.width, desc.height);
        } else {
            context.TextureStorage3D(id, desc.mipLevels+1, textureFormatsToGl(desc.format), desc.width, desc.height, desc.depth);
        }
    }
    TextureFormat Texture::getFormat() {return desc.format;}
}