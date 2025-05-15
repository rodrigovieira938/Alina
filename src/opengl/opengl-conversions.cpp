#include "glad/gl.h"
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
}