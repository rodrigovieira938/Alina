#pragma once
#include <array>
#include <memory>
#include <string>
#include <stdint.h>
#include <vector>

namespace alina {
    // ===================
    // Handles
    // ===================
    class IBuffer;
    class IInputLayout;
    class IGraphicsPipeline;
    class IShader;
    class ISampler;
    class ITexture;
    class IFramebuffer;
    class ICommandList;
    class IDevice;

    using Buffer = std::shared_ptr<IBuffer>;
    using InputLayout = std::shared_ptr<IInputLayout>;
    using GraphicsPipeline = std::shared_ptr<IGraphicsPipeline>;
    using Shader = std::shared_ptr<IShader>;
    using Sampler = std::shared_ptr<ISampler>;
    using Texture = std::shared_ptr<ITexture>;
    using Framebuffer = std::shared_ptr<IFramebuffer>;
    using CommandList = std::shared_ptr<ICommandList>;
    using Device = std::shared_ptr<IDevice>;
    // ===================
    // Enums
    // ===================
    enum class BufferType {
        VERTEX,
        INDEX,
        UNIFORM
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
    enum class VertexAttributeFormat {
        Int,
        Unsigned_Int,
        Float,
    };
    enum class ShaderType {
        //TODO: add more
        VERTEX,
        FRAGMENT
    };
    enum class TextureFormat {
        #define FORMAT(type, bits) \
            R    ## bits ## _ ## type, \
            RG   ## bits ## _ ## type, \
            RGB  ## bits ## _ ## type, \
            RGBA ## bits ## _ ## type 
        Unknown,
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
    enum class TextureFilter {
        // Samples the same as in the texture  
        Point,
        // Samples are averaged
        Bilinear,
        // Sample are averaged and blended between mipmaps
        Trilinear
    };
    enum class TextureWrap {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
    };
    enum class TextureUsage {
        TEXTURE,
        ATTACHMENT,
    };
    enum class RenderPassLoadOp {
        LOAD, CLEAR
    };
    enum class SubPassAttachment {
        NONE,
        PRESERVE,
        INPUT,
        COLOR,
    };
    // ===================
    // Structs
    // ===================
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
        Buffer buffer = 0;
        uint32_t offset = 0, stride = 0;

        BindVertexBuffer& setBuffer(Buffer  value) {buffer = value; return *this;}
        BindVertexBuffer& setOffset(uint32_t value) {offset = value; return *this;}
        BindVertexBuffer& setStride(uint32_t value) {stride = value; return *this;}
    };
    struct Color {
        float r=.0f,g=.0f,b=.0f,a = 1.0;
    };
    struct FramebufferAttachment {
        Texture texture;
        FramebufferAttachment& setTexture(Texture value) {texture = value; return *this;}
    };
    struct UniformBufferBinding {
        Buffer buffer = nullptr;
        uint32_t set = 0, binding = 0;
        
        UniformBufferBinding& setBuffer(Buffer value) {buffer = value; return *this;}
        UniformBufferBinding& setSet(uint32_t value) {set = value; return *this;}
        UniformBufferBinding& setBinding(uint32_t value) {binding = value; return *this;}
    };
    struct TextureBinding {
        Sampler sampler = nullptr;
        Texture texture = nullptr;
        uint32_t set = 0, binding = 0;

        TextureBinding& setTexture(Texture value) {texture = value; return *this;}
        TextureBinding& setSampler(Sampler value) {sampler = value; return *this;}
        TextureBinding& setSet(uint32_t value) {set = value; return *this;}
        TextureBinding& setBinding(uint32_t value) {binding = value; return *this;}
    };
    struct ShaderResources {
        std::vector<UniformBufferBinding> uboBinding;
        std::vector<TextureBinding> texBinding;

        ShaderResources& setUboBindings(const std::vector<UniformBufferBinding> value) {uboBinding = value; return *this;}
        ShaderResources& setTexBindings(const std::vector<TextureBinding> value)       {texBinding = value; return *this;}
    };
    // ===================
    // Interface Descs
    // ===================

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
    //Simple graphics pipeline desc, must be expanded
    struct GraphicsPipelineDesc {
        PrimitiveType primType = PrimitiveType::TriangleList;
        RenderState renderState;
        InputLayout inputLayout = nullptr;
        Shader vs = nullptr,fs = nullptr;

        GraphicsPipelineDesc& setPrimType(PrimitiveType type) { primType = type; return *this; }
        GraphicsPipelineDesc& setRenderState(RenderState state) { renderState = state; return *this; }
        GraphicsPipelineDesc& setInputLayout(InputLayout value) { inputLayout = value; return *this; }
        GraphicsPipelineDesc& setVertexShader(Shader value) { vs = value; return *this; }
        GraphicsPipelineDesc& setFragmentShader(Shader value) { fs = value; return *this; }
    };
    struct SamplerDesc {
        TextureFilter filter = TextureFilter::Bilinear;
        bool minFilter = true, magFilter = true, mipFilter = true;
        TextureWrap u = TextureWrap::ClampToEdge,v = TextureWrap::ClampToEdge,w = TextureWrap::ClampToEdge;
        Color border = {1,1,1,1};

        SamplerDesc& setFilter(TextureFilter value) {filter = value; return *this;}
        SamplerDesc& setAllFilters(bool value) {minFilter = magFilter = mipFilter = value; return *this;}
        SamplerDesc& setMinFilter(bool value) {minFilter = value; return *this;}
        SamplerDesc& setMagFilter(bool value) {magFilter = value; return *this;}
        SamplerDesc& setMipFilter(bool value) {mipFilter = value; return *this;}
        SamplerDesc& setAllWraps(TextureWrap value) {u = v = w = value; return *this;}
        SamplerDesc& setUWrap(TextureWrap value) {u = value; return *this;}
        SamplerDesc& setVWrap(TextureWrap value) {v = value; return *this;}
        SamplerDesc& setWWrap(TextureWrap value) {w = value; return *this;}
        SamplerDesc& setAllWrap(TextureWrap value) {u = v = w = value; return *this;}
    };
    //TODO: add clearColor
    struct TextureDesc {
        TextureFormat format = TextureFormat::RGB8_Unorm;
        uint32_t width = 1, height = 1;
        uint32_t depth = 1;
        // 0 -> none
        uint32_t mipLevels = 0;
        SamplerDesc sampler;
        TextureUsage usage = TextureUsage::TEXTURE;
        std::string name; 

        TextureDesc& setFormat(TextureFormat value) {format = value; return *this;}
        TextureDesc& setWidth(uint32_t value) {width = value; return *this;}
        TextureDesc& setHeight(uint32_t value) {height = value; return *this;}
        TextureDesc& setDepth(uint32_t value) {depth = value; return *this;}
        TextureDesc& setMipLevels(uint32_t value) {mipLevels = value; return *this;}
        TextureDesc& setSampler(const SamplerDesc& value) {sampler = value; return *this;}
        TextureDesc& setUsage(TextureUsage value) {usage = value; return *this;}
        TextureDesc& setDebugName(std::string name) {
            this->name = name;
            return *this;
        };
    };
    //TODO: add acess to only a specific layer
    struct FramebufferDesc {
        std::array<FramebufferAttachment, 8> colorAttachments;
        FramebufferAttachment depthAttachment;
        std::string name;

        FramebufferDesc& setColorAttachments(const std::array<FramebufferAttachment, 8>& value) {colorAttachments = value; return *this;}
        FramebufferDesc& setDepthAttachment(FramebufferAttachment value) {depthAttachment = value; return *this;}
        FramebufferDesc& setDebugName(std::string name) {
            this->name = name;
            return *this;
        };
    };
    struct RenderPassDesc {
        Framebuffer fb = nullptr;
        std::array<RenderPassLoadOp, 8> loadOps;
        std::array<std::array<float, 4>, 8> clearColors;
        RenderPassDesc& setFramebuffer(Framebuffer value) {fb = value; return *this;}
        RenderPassDesc& setAttachmentsLoadOp(const std::array<RenderPassLoadOp, 8>& value) {loadOps = value; return *this;}
        RenderPassDesc& setAttachmentsClearColors(const std::array<std::array<float, 4>, 8>& value) {clearColors = value; return *this;}
    
    };
    struct SubPassDesc {
        std::array<SubPassAttachment, 8> attachments;

        SubPassDesc& setAttachments(const std::array<SubPassAttachment, 8>& value) {attachments = value; return *this;}
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
        VertexAttributeDesc& setOffset(uint32_t value) {offset = value; return *this;}
        VertexAttributeDesc& setStride(uint32_t value) {stride = value; return *this;}
        VertexAttributeDesc& setFormat(VertexAttributeFormat value) {format = value; return *this;}
        VertexAttributeDesc& setInstanced(bool value) {isInstanced = value; return *this;}

    };
    // ===================
    // Interfaces
    // ===================
    class IObject {
    public:
        virtual ~IObject() = default;
        virtual IDevice* getDevice() = 0;
    };
    class INameableObject : public IObject {
    public:
        virtual ~INameableObject() = default;
        virtual std::string getName() = 0;
        virtual void setName(const std::string& name) = 0;
    };

    class IBuffer : public INameableObject{
    public:
        virtual ~IBuffer() = default;
        virtual BufferType getType() const = 0; 
    };

    class IInputLayout : public INameableObject{
    public:
        virtual ~IInputLayout() = default;
        virtual VertexAttributeDesc* getAttributes() = 0;
        virtual size_t getNumAttributes() = 0;
    };
    class IShader : public INameableObject{
    public:
        virtual ~IShader() = default;
        virtual ShaderType getType() = 0;
    };
    class IGraphicsPipeline : public INameableObject{
    public:
        virtual ~IGraphicsPipeline() = default;
    };
    class ISampler : public INameableObject{
    public:
        virtual ~ISampler() = default;
    };
    class ITexture : public INameableObject{
    public:
        virtual ~ITexture() = default;
        virtual TextureFormat getFormat() = 0;
    };
    class IFramebuffer : public INameableObject{
    public:
        virtual ~IFramebuffer() = default;
    };
    class ICommandList : public INameableObject{
    public:
        virtual ~ICommandList() = default;
        virtual void begin() = 0;
        virtual void bindGraphicsPipeline(GraphicsPipeline pipeline) = 0;
        virtual void bindShaderResources(const ShaderResources& shaderResouces) = 0;
        virtual void bindVertexBuffers(const std::vector<BindVertexBuffer>& buffers) = 0;
        virtual void bindIndexBuffer(Buffer buffer) = 0;
        virtual void draw(const DrawArguments& drawArgs) = 0;
        virtual void drawIndexed(const DrawArguments& drawArgs) = 0;
        virtual void writeBuffer(Buffer buffer, const void* data, size_t size, size_t offset) = 0;
        virtual void clearBuffer(Buffer buffer, uint32_t clearValue) = 0;
        //TODO: add offsets and portions of the texture
        virtual void blitTexture(Texture src, Texture dest) = 0;
        virtual void generateMipMaps(Texture tex) = 0;
        //TODO: add offsets and portions of the texture
        virtual void writeTexture(Texture tex, const void* data, TextureFormat dataFormat) = 0;
        virtual void beginRenderPass(const RenderPassDesc& desc) = 0;
        virtual void beginSubPass(const SubPassDesc& desc) = 0;
        virtual void endRenderPass() = 0;
        virtual void end() = 0;
    };
    class IDevice{
    public:
        virtual ~IDevice() = default;
        virtual bool beginFrame() = 0;
        virtual void endFrame() = 0;
        virtual Buffer createBuffer(const BufferDesc& desc) = 0;
        virtual CommandList createCommandList() = 0;
        virtual InputLayout createInputLayout(const VertexAttributeDesc* attrs, size_t size) = 0;
        inline InputLayout createInputLayout(const std::vector<VertexAttributeDesc>& attrs) {return createInputLayout(attrs.data(), attrs.size());}
        //Raw Shader data, for opengl -> glsl text, for vulkan -> spirv
        virtual Shader createShader(ShaderType type, const void* data, size_t size) = 0;
        virtual GraphicsPipeline createGraphicsPipeline(const GraphicsPipelineDesc& desc) = 0;
        virtual Texture createTexture(const TextureDesc& desc) = 0;
        virtual Sampler createSampler(const SamplerDesc& desc) = 0;
        virtual Framebuffer createFramebuffer(const FramebufferDesc& desc) = 0;
        virtual void execute(CommandList cmd) = 0;
    };
}