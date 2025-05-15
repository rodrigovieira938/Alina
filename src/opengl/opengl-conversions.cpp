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
            #define API_FORMAT_R(type, bits) GL_R ## bits ## _ ## type
            #define API_FORMAT_RG(type, bits) GL_RG ## bits ## _ ## type
            #define API_FORMAT_RGB(type, bits) GL_RGB ## bits ## _ ## type
            #define API_FORMAT_RGBA(type, bits) GL_RGBA ## bits ## _ ## type
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
            case TextureFormat::SRGB8:
                return GL_SRGB8;
            case TextureFormat::SRGBA8:
                return GL_SRGB_ALPHA;
        }
        return -1;
    }
}