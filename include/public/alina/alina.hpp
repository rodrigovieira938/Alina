#pragma once
#include <string>
#include <stdint.h>
#include <vector>

namespace alina {
    enum class BufferType {
        VERTEX,
        INDEX,
        UNIFORM
    };
    struct BufferDesc {
        BufferDesc& setDebugName(std::string name) {
            this->name = name;
            return *this;
        };
        BufferDesc& setType(BufferType type) {
            this->type = type;
            return *this;
        };
        std::string name = "";
        BufferType type = BufferType::VERTEX;
    };
    class Buffer {
    public:
        virtual BufferType getType() const = 0; 
    };
    enum class PrimitiveType : uint8_t
    {
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,
        TriangleFan,
        TriangleListWithAdjacency,
        TriangleStripWithAdjacency,
        PatchList
    };
    enum class RasterFillMode : uint8_t
    {
        Solid,
        Wireframe,

        // Vulkan names
        Fill = Solid,
        Line = Wireframe
    };

    enum class RasterCullMode : uint8_t
    {
        Back,
        Front,
        None
    };

    struct RasterState
    {
        RasterFillMode fillMode = RasterFillMode::Solid;
        RasterCullMode cullMode = RasterCullMode::Back;
        bool frontCounterClockwise = false;
        RasterState& setFillMode(RasterFillMode value) { fillMode = value; return *this; }
        RasterState& setFillSolid() { fillMode = RasterFillMode::Solid; return *this; }
        RasterState& setFillWireframe() { fillMode = RasterFillMode::Wireframe; return *this; }
        RasterState& setCullMode(RasterCullMode value) { cullMode = value; return *this; }
        RasterState& setCullBack() { cullMode = RasterCullMode::Back; return *this; }
        RasterState& setCullFront() { cullMode = RasterCullMode::Front; return *this; }
        RasterState& setCullNone() { cullMode = RasterCullMode::None; return *this; }
        RasterState& setFrontCounterClockwise(bool value) { frontCounterClockwise = value; return *this; }
    };
    struct RenderState {
        RasterState rasterState;

        RenderState& setRasterState(const RasterState& value) { rasterState = value; return *this; }
    };
    enum class VertexAttributeFormat {
        Int,
        Unsigned_Int,
        Float,
    };
    struct VertexAttributeDesc {
        std::string name;
        uint32_t bufferIndex = 0;
        uint32_t arraySize = 1;
        uint32_t offset = 0;
        uint32_t stride = 0;
        bool isInstanced = false;
        VertexAttributeFormat format;

        VertexAttributeDesc& setName(std::string value) {name = value; return *this;}
        VertexAttributeDesc& setBufferIndex(uint32_t value) {bufferIndex = value; return *this;}
        VertexAttributeDesc& setArraySize(uint32_t value) {arraySize = value; return *this;}
        VertexAttributeDesc& setOffset(uint32_t value) {bufferIndex = value; return *this;}
        VertexAttributeDesc& setStride(uint32_t value) {stride = value; return *this;}
        VertexAttributeDesc& setFormat(VertexAttributeFormat value) {format = value; return *this;}
        VertexAttributeDesc& setInstanced(bool value) {isInstanced = value; return *this;}

    };
    class InputLayout {
    public:
        virtual VertexAttributeDesc* getAttributes() = 0;
        virtual size_t getNumAttributes() = 0;
    };
    enum class ShaderType {
        //TODO: add more
        VERTEX,
        FRAGMENT
    };
    class Shader {
        virtual ShaderType getType() = 0;
    };
    //Simple graphics pipeline desc, must be expanded
    struct GraphicsPipelineDesc {
        PrimitiveType primType = PrimitiveType::TriangleList;
        RenderState renderState;
        InputLayout* inputLayout = nullptr;
        Shader *vs = nullptr,*fs = nullptr;

        GraphicsPipelineDesc& setPrimType(PrimitiveType type) { primType = type; return *this; }
        GraphicsPipelineDesc& setRenderState(RenderState state) { renderState = state; return *this; }
        GraphicsPipelineDesc& setInputLayout(InputLayout* value) { inputLayout = value; return *this; }
        GraphicsPipelineDesc& setVertexShader(Shader* value) { vs = value; return *this; }
        GraphicsPipelineDesc& setFragmentShader(Shader* value) { fs = value; return *this; }
    };
    class GraphicsPipeline {
    };
    struct DrawArguments
    {
        uint32_t vertexCount = 0;
        uint32_t offset = 0;
        uint32_t instanceCount = 1;

        DrawArguments& setVertexCount(uint32_t value) { vertexCount = value; return *this; }
        DrawArguments& setOffset(uint32_t value) { offset = value; return *this; }
        DrawArguments& setInstanceCount(uint32_t value) { instanceCount = value; return *this; }
    };
    struct BindVertexBuffer {
        Buffer* buffer = 0;
        uint32_t offset = 0, stride = 0;

        BindVertexBuffer& setBuffer(Buffer*  value) {buffer = value; return *this;}
        BindVertexBuffer& setOffset(uint32_t value) {offset = value; return *this;}
        BindVertexBuffer& setStride(uint32_t value) {stride = value; return *this;}
    };
    struct UniformBufferBinding {
        Buffer* buffer = nullptr;
        uint32_t set = 0, binding = 0;
        
        UniformBufferBinding& setBuffer(Buffer* value) {buffer = value; return *this;}
        UniformBufferBinding& setSet(uint32_t value) {set = value; return *this;}
        UniformBufferBinding& setBinding(uint32_t value) {binding = value; return *this;}
    };
    enum class TextureFormat {
        #define FORMAT(type, bits) \
            R    ## bits ## _ ## type, \
            RG   ## bits ## _ ## type, \
            RGB  ## bits ## _ ## type, \
            RGBA ## bits ## _ ## type 

        FORMAT(Sint, 8),
        FORMAT(Sint, 16),
        FORMAT(Sint, 32),
        FORMAT(Uint, 8),
        FORMAT(Uint, 16),
        FORMAT(Uint, 32),
        FORMAT(Snorm, 8),
        FORMAT(Snorm, 16),
        FORMAT(Unorm, 8),
        FORMAT(Unorm, 16),
        FORMAT(Float, 16),
        FORMAT(Float, 32),
        
        SRGB8,
        SRGBA8

        #undef FORMAT
    };
    //TODO: add clearColor, add samplerDesc and sampler object
    struct TextureDesc {
        TextureFormat format = TextureFormat::RGB8_Unorm;
        uint32_t width = 1, height = 1;
        uint32_t depth = 1;
        // 0 -> none
        uint32_t mipLevels = 0;

        TextureDesc& setFormat(TextureFormat value) {format = value; return *this;}
        TextureDesc& setWidth(uint32_t value) {width = value; return *this;}
        TextureDesc& setHeight(uint32_t value) {height = value; return *this;}
        TextureDesc& setDepth(uint32_t value) {width = value; return *this;}
        TextureDesc& setMipLevels(uint32_t value) {mipLevels = value; return *this;}
    };
    class Texture {
    public:
        virtual TextureFormat getFormat() = 0;
    };
    struct ShaderResources {
        std::vector<UniformBufferBinding> uboBinding;

        ShaderResources& setUboBindings(const std::vector<UniformBufferBinding> value) {uboBinding = value; return *this;}
    };
    class CommandList {
    public:
        virtual void begin() = 0;
        virtual void bindGraphicsPipeline(GraphicsPipeline* pipeline) = 0;
        virtual void bindShaderResources(const ShaderResources& shaderResouces) = 0;
        virtual void bindVertexBuffers(const std::vector<BindVertexBuffer>& buffers) = 0;
        virtual void bindIndexBuffer(Buffer* buffer) = 0;
        virtual void draw(const DrawArguments& drawArgs) = 0;
        virtual void drawIndexed(const DrawArguments& drawArgs) = 0;
        virtual void writeBuffer(Buffer* buffer, const void* data, size_t size, size_t offset) = 0;
        virtual void clearBuffer(Buffer* buffer, uint32_t clearValue) = 0;
        virtual void end() = 0;
    };
    class Device {
    public:
        virtual bool beginFrame() = 0;
        virtual void endFrame() = 0;
        virtual Buffer* createBuffer(const BufferDesc& desc) = 0;
        virtual CommandList* createCommandList() = 0;
        virtual InputLayout* createInputLayout(const VertexAttributeDesc* attrs, size_t size) = 0;
        inline InputLayout* createInputLayout(const std::vector<VertexAttributeDesc>& attrs) {return createInputLayout(attrs.data(), attrs.size());}
        //Raw Shader data, for opengl -> glsl text, for vulkan -> spirv
        virtual Shader* createShader(ShaderType type, const void* data, size_t size) = 0;
        virtual GraphicsPipeline* createGraphicsPipeline(const GraphicsPipelineDesc& desc) = 0;
        virtual Texture* createTexture(const TextureDesc& desc) = 0;
        virtual void execute(CommandList* cmd) = 0;
    };
}