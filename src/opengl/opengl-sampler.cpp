#include <alina/opengl-sampler.hpp>
#include <alina/opengl-device.hpp>
#include <alina/opengl-conversions.hpp>

namespace alina::opengl {
    GlSampler::GlSampler(const SamplerDesc& desc, GlDevice* device) {
        auto context = device->context;
        context.CreateSamplers(1, &id);
        if(desc.magFilter) {
            //Mipmaps filtering on mag is not permitted
            context.SamplerParameteri(id, GL_TEXTURE_MAG_FILTER, textureFilterToGl(desc.filter, false));
        }
        if(desc.minFilter) {
            context.SamplerParameteri(id, GL_TEXTURE_MIN_FILTER, textureFilterToGl(desc.filter, desc.mipFilter));
        }
        context.SamplerParameteri(id, GL_TEXTURE_WRAP_S, textureWrapToGl(desc.u));
        context.SamplerParameteri(id, GL_TEXTURE_WRAP_T, textureWrapToGl(desc.v));
        context.SamplerParameteri(id, GL_TEXTURE_WRAP_R, textureWrapToGl(desc.w));
    
        if(desc.u == TextureWrap::ClampToBorder || desc.v == TextureWrap::ClampToBorder || desc.w == TextureWrap::ClampToBorder)
            context.SamplerParameterfv(id, GL_TEXTURE_BORDER_COLOR, &desc.border.r);
    }
}