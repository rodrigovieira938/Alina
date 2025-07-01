#include "alina/alina.hpp"
#include "glad/gl.h"
#include <alina/opengl-texture.hpp>
#include <alina/opengl-device.hpp>
#include <alina/opengl-conversions.hpp>
#include <math.h>

namespace alina::opengl {
    Texture::Texture(const TextureDesc& _desc, IDevice* device) : desc(_desc) {
        auto context = ((Device*)device)->context;
        if(desc.mipLevels == 0)
            desc.sampler.mipFilter = false;
        context.CreateTextures(desc.depth == 1 ? GL_TEXTURE_2D : GL_TEXTURE_3D, 1, &id);
        if(desc.depth == 1) {
            context.TextureStorage2D(id, desc.mipLevels+1, textureFormatsToGl(desc.format), desc.width, desc.height);
        } else {
            context.TextureStorage3D(id, desc.mipLevels+1, textureFormatsToGl(desc.format), desc.width, desc.height, desc.depth);
        }

        if(desc.sampler.magFilter) {
            //Mipmaps filtering on mag is not permitted
            context.TextureParameteri(id, GL_TEXTURE_MAG_FILTER, textureFilterToGl(desc.sampler.filter, false));
        }
        if(desc.sampler.minFilter) {
            context.TextureParameteri(id, GL_TEXTURE_MIN_FILTER, textureFilterToGl(desc.sampler.filter, desc.sampler.mipFilter));
        }
        context.TextureParameteri(id, GL_TEXTURE_WRAP_S, textureWrapToGl(desc.sampler.u));
        context.TextureParameteri(id, GL_TEXTURE_WRAP_T, textureWrapToGl(desc.sampler.v));
        context.TextureParameteri(id, GL_TEXTURE_WRAP_R, textureWrapToGl(desc.sampler.w));
    
        if(desc.sampler.u == TextureWrap::ClampToBorder || desc.sampler.v == TextureWrap::ClampToBorder || desc.sampler.w == TextureWrap::ClampToBorder)
            context.TextureParameterfv(id, GL_TEXTURE_BORDER_COLOR, &desc.sampler.border.r);
    }
    TextureFormat Texture::getFormat() {return desc.format;}
}