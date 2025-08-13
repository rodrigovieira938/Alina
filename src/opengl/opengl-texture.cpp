#include "alina/alina.hpp"
#include "glad/gl.h"
#include <alina/opengl-texture.hpp>
#include <alina/opengl-device.hpp>
#include <alina/opengl-conversions.hpp>
#include <math.h>

namespace alina::opengl {
    GlTexture::GlTexture(const TextureDesc& _desc, GlDevice* device) : desc(_desc) {
        auto context = device->context;
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
        context.ObjectLabel(GL_TEXTURE, id, desc.name.size(), desc.name.data());
    }
    GlTexture::GlTexture(uint32_t id, GlDevice* device) {
        auto context = device->context;
        GLint internalFormat = 0, width = 0, height = 0, depth = 0, mipLevels = 0;
        context.GetTextureLevelParameteriv(id, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
        context.GetTextureLevelParameteriv(id, 0, GL_TEXTURE_WIDTH, &width);
        context.GetTextureLevelParameteriv(id, 0, GL_TEXTURE_HEIGHT, &height);
        context.GetTextureLevelParameteriv(id, 0, GL_TEXTURE_DEPTH, &depth);
        //TODO: find another way to get the mipmapLevels
        context.GetTextureParameteriv(id, GL_TEXTURE_MAX_LEVEL, &mipLevels);

        desc.format = glTextureFormatToAlina(internalFormat);
        desc.width = width;
        desc.height = height;
        desc.mipLevels = mipLevels + 1;
        this->id = id;
    }
    TextureFormat GlTexture::getFormat() {return desc.format;}
}