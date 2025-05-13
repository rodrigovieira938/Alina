#pragma once
#include <string>
#include <stdint.h>

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
    //Simple graphics pipeline desc, must be expanded
    struct GraphicsPipelineDesc {
        PrimitiveType primType = PrimitiveType::TriangleList;
        RenderState renderState;

        GraphicsPipelineDesc& setPrimType(PrimitiveType type) { primType = type; return *this; }
        GraphicsPipelineDesc& setRenderState(RenderState state) { renderState = state; return *this; }
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
    class CommandList {
    public:
        virtual void begin() = 0;
        virtual void bindGraphicsPipeline(GraphicsPipeline* pipeline) = 0;
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
        virtual GraphicsPipeline* createGraphicsPipeline(const GraphicsPipelineDesc& desc) = 0;
        virtual void execute(CommandList* cmd) = 0;
    };
}