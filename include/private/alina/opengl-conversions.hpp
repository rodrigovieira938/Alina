#include <glad/gl.h>
#include <alina/alina.hpp>

namespace alina::opengl {
    GLenum bufferTypeToGl(const BufferType& type);
    GLenum rasterFillModeToGl(const RasterFillMode& mode);
    GLenum rasterCullModeToGl(const RasterCullMode& mode);
    GLenum vertexAttrFormatToGl(const VertexAttributeFormat& format);
    GLenum shaderTypeToGl(const ShaderType& type);
    GLenum textureFormatsToGl(const TextureFormat& format);
    GLenum textureFormatsToGlChannels(const TextureFormat& format);
    GLenum textureFormatsToGlType(const TextureFormat& format);
    GLenum textureFilterToGl(const TextureFilter& filter, bool mips);
    GLenum textureWrapToGl(const TextureWrap& wrap);
}