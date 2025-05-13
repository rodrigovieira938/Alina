#include <glad/gl.h>
#include <alina/alina.hpp>

namespace alina::opengl {
    GLenum bufferTypeToGl(const BufferType& type);
    GLenum rasterFillModeToGl(const RasterFillMode& mode);
    GLenum rasterCullModeToGl(const RasterCullMode& mode);
    GLenum vertexAttrFormatToGl(const VertexAttributeFormat& format);
    GLenum shaderTypeToGl(const ShaderType& type);
}