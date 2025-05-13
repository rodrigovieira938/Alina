#pragma once
#include <string>
#include <stdint.h>
#include <vector>

namespace alina {
    enum class BufferType {
        VERTEX,
        INDEX
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

        DrawArguments& setVertexCount(uint32_t value) { vertexCount = value; return *this; }
        DrawArguments& setOffset(uint32_t value) { offset = value; return *this; }
    };
    struct BindVertexBuffer {
        Buffer* buffer = 0;
        uint32_t offset = 0, stride = 0;

        BindVertexBuffer& setBuffer(Buffer*  value) {buffer = value; return *this;}
        BindVertexBuffer& setOffset(uint32_t value) {offset = value; return *this;}
        BindVertexBuffer& setStride(uint32_t value) {stride = value; return *this;}
    };
    class CommandList {
    public:
        virtual void begin() = 0;
        virtual void bindGraphicsPipeline(GraphicsPipeline* pipeline) = 0;
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
        virtual void execute(CommandList* cmd) = 0;
    };
}