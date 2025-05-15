#include "alina/alina.hpp"
#include "glad/gl.h"
#include <GL/glext.h>
#include <alina/opengl-conversions.hpp>
namespace alina::opengl {
    GLenum bufferTypeToGl(const BufferType& type) {
        switch (type) {
            case BufferType::VERTEX:
                return GL_ARRAY_BUFFER;
            case BufferType::INDEX:
                return GL_ELEMENT_ARRAY_BUFFER;
            case BufferType::UNIFORM:
                return GL_UNIFORM_BUFFER;
        }
        return -1;
    }
    GLenum rasterFillModeToGl(const RasterFillMode& mode) {
        switch(mode) {
        case RasterFillMode::Solid:
            return GL_FILL;
        case RasterFillMode::Wireframe:
            return GL_LINE;            
        }
        return -1;
    }
    GLenum rasterCullModeToGl(const RasterCullMode& mode) {
        switch(mode) {
        case RasterCullMode::Back:
            return GL_BACK;
        case RasterCullMode::Front:
            return GL_FRONT;
        case RasterCullMode::None:
            return GL_FRONT_AND_BACK;
        }
        return -1;
    }
    GLenum vertexAttrFormatToGl(const VertexAttributeFormat& format) {
        switch (format) {
        case VertexAttributeFormat::Int:
            return GL_INT;
        case VertexAttributeFormat::Unsigned_Int:
            return GL_UNSIGNED_INT;
        case VertexAttributeFormat::Float:
            return GL_FLOAT;
        }
        return -1;
    }
    GLenum shaderTypeToGl(const ShaderType& type) {
        switch(type) {
        case ShaderType::VERTEX:
            return GL_VERTEX_SHADER;
        case ShaderType::FRAGMENT:
            return GL_FRAGMENT_SHADER;
        }
        return -1;
    }
    GLenum textureFormatsToGl(const TextureFormat& format) {
        switch(format) {
            #define FORMAT(type, bits, apitype) \
            case TextureFormat::R    ## bits ## _ ## type: return GL_R ## bits ## _ ## apitype; \
            case TextureFormat::RG   ## bits ## _ ## type: return GL_RG ## bits ## _ ## apitype; \
            case TextureFormat::RGB  ## bits ## _ ## type: return GL_RGB ## bits ## _ ## apitype; \
            case TextureFormat::RGBA ## bits ## _ ## type: return GL_RGBA ## bits ## _ ## apitype; \

            #define FORMAT2(type, bits, apitype) \
            case TextureFormat::R    ## bits ## _ ## type: return GL_R ## bits ## apitype; \
            case TextureFormat::RG   ## bits ## _ ## type: return GL_RG ## bits ## apitype; \
            case TextureFormat::RGB  ## bits ## _ ## type: return GL_RGB ## bits ## apitype; \
            case TextureFormat::RGBA ## bits ## _ ## type: return GL_RGBA ## bits ## apitype; \

            #define FORMAT3(type, bits, apitype, apibits) \
            case TextureFormat::R    ## bits ## _ ## type: return GL_R ## apibits ## apitype; \
            case TextureFormat::RG   ## bits ## _ ## type: return GL_RG ## apibits ## apitype; \
            case TextureFormat::RGB  ## bits ## _ ## type: return GL_RGB ## apibits ## apitype; \
            case TextureFormat::RGBA ## bits ## _ ## type: return GL_RGBA ## apibits ## apitype; \

            FORMAT2(Sint, 8, I)
            FORMAT2(Sint, 16, I)
            FORMAT2(Sint, 32, I)
            FORMAT2(Uint, 8, UI)
            FORMAT2(Uint, 16, UI)
            FORMAT2(Uint, 32, UI)
            FORMAT(Snorm, 8, SNORM)
            FORMAT(Snorm, 16, SNORM)
            FORMAT3(Unorm, 8, F, 32)
            FORMAT3(Unorm, 16, F, 32)
            FORMAT3(Float, 16, F, 32)
            FORMAT2(Float, 32, F)
            #undef FORMAT
            #undef FORMAT2
            #undef FORMAT3
            case TextureFormat::SRGB8:
                return GL_SRGB8;
            case TextureFormat::SRGBA8:
                return GL_SRGB_ALPHA;
        }
        return -1;
    }
    //TODO: handle unsupported formats
    GLenum textureFormatsToGlChannels(const TextureFormat& format) {
        switch(format) {
            #define FORMAT(type, bits) \
            case TextureFormat::R    ## bits ## _ ## type: return GL_RED; \
            case TextureFormat::RG   ## bits ## _ ## type: return GL_RG; \
            case TextureFormat::RGB  ## bits ## _ ## type: return GL_RGB; \
            case TextureFormat::RGBA ## bits ## _ ## type: return GL_RGBA;
            FORMAT(Sint, 8)
            FORMAT(Sint, 16)
            FORMAT(Sint, 32)
            FORMAT(Uint, 8)
            FORMAT(Uint, 16)
            FORMAT(Uint, 32)
            FORMAT(Snorm, 8)
            FORMAT(Snorm, 16)
            FORMAT(Unorm, 8)
            FORMAT(Unorm, 16)
            FORMAT(Float, 16)
            FORMAT(Float, 32)
            #undef FORMAT
            case TextureFormat::SRGB8:
                return GL_RGB;
            case TextureFormat::SRGBA8:
                return GL_RGBA;
        }
        return -1;
    }
    //TODO: handle unsupported formats
    GLenum textureFormatsToGlType(const TextureFormat& format) {
        switch(format) {
            #define FORMAT(type, bits, rtype) \
            case TextureFormat::R    ## bits ## _ ## type: return rtype; \
            case TextureFormat::RG   ## bits ## _ ## type: return rtype; \
            case TextureFormat::RGB  ## bits ## _ ## type: return rtype; \
            case TextureFormat::RGBA ## bits ## _ ## type: return rtype;
            FORMAT(Sint, 8, GL_BYTE)
            FORMAT(Sint, 16, GL_SHORT)
            FORMAT(Sint, 32, GL_INT)
            FORMAT(Uint, 8, GL_UNSIGNED_INT)
            FORMAT(Uint, 16, GL_UNSIGNED_SHORT)
            FORMAT(Uint, 32, GL_UNSIGNED_INT)
            FORMAT(Snorm, 8, GL_BYTE)
            FORMAT(Snorm, 16, GL_SHORT)
            FORMAT(Unorm, 8, GL_UNSIGNED_BYTE)
            FORMAT(Unorm, 16, GL_UNSIGNED_SHORT)
            FORMAT(Float, 16, GL_HALF_FLOAT)
            FORMAT(Float, 32, GL_FLOAT)
            #undef FORMAT
            case TextureFormat::SRGB8:
                return GL_RGB;
            case TextureFormat::SRGBA8:
                return GL_RGBA;
        }
        return -1;
    }

}