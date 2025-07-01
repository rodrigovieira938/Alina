#pragma once
#include <array>
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
    class IBuffer {
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
        VertexAttributeDesc& setOffset(uint32_t value) {offset = value; return *this;}
        VertexAttributeDesc& setStride(uint32_t value) {stride = value; return *this;}
        VertexAttributeDesc& setFormat(VertexAttributeFormat value) {format = value; return *this;}
        VertexAttributeDesc& setInstanced(bool value) {isInstanced = value; return *this;}

    };
    class IInputLayout {
    public:
        virtual VertexAttributeDesc* getAttributes() = 0;
        virtual size_t getNumAttributes() = 0;
    };
    enum class ShaderType {
        //TODO: add more
        VERTEX,
        FRAGMENT
    };
    class IShader {
        virtual ShaderType getType() = 0;
    };
    //Simple graphics pipeline desc, must be expanded
    struct GraphicsPipelineDesc {
        PrimitiveType primType = PrimitiveType::TriangleList;
        RenderState renderState;
        IInputLayout* inputLayout = nullptr;
        IShader *vs = nullptr,*fs = nullptr;

        GraphicsPipelineDesc& setPrimType(PrimitiveType type) { primType = type; return *this; }
        GraphicsPipelineDesc& setRenderState(RenderState state) { renderState = state; return *this; }
        GraphicsPipelineDesc& setInputLayout(IInputLayout* value) { inputLayout = value; return *this; }
        GraphicsPipelineDesc& setVertexShader(IShader* value) { vs = value; return *this; }
        GraphicsPipelineDesc& setFragmentShader(IShader* value) { fs = value; return *this; }
    };
    class IGraphicsPipeline {
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
        IBuffer* buffer = 0;
        uint32_t offset = 0, stride = 0;

        BindVertexBuffer& setBuffer(IBuffer*  value) {buffer = value; return *this;}
        BindVertexBuffer& setOffset(uint32_t value) {offset = value; return *this;}
        BindVertexBuffer& setStride(uint32_t value) {stride = value; return *this;}
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
    struct Color {
        float r=.0f,g=.0f,b=.0f,a = 1.0;
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
    enum class TextureUsage {
        TEXTURE,
        ATTACHMENT,
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

        TextureDesc& setFormat(TextureFormat value) {format = value; return *this;}
        TextureDesc& setWidth(uint32_t value) {width = value; return *this;}
        TextureDesc& setHeight(uint32_t value) {height = value; return *this;}
        TextureDesc& setDepth(uint32_t value) {width = value; return *this;}
        TextureDesc& setMipLevels(uint32_t value) {mipLevels = value; return *this;}
        TextureDesc& setSampler(const SamplerDesc& value) {sampler = value; return *this;}
        TextureDesc& setUsage(TextureUsage value) {usage = value; return *this;}
    };
    class ISampler {

    };
    class ITexture {
    public:
        virtual TextureFormat getFormat() = 0;
    };
    struct FramebufferAttachment {
        ITexture* texture;
        FramebufferAttachment& setTexture(ITexture* value) {texture = value; return *this;}
    };
    //TODO: add acess to only a specific layer
    struct FramebufferDesc {
        std::array<FramebufferAttachment, 8> colorAttachments;
        FramebufferAttachment depthAttachment;

        FramebufferDesc& setColorAttachments(const std::array<FramebufferAttachment, 8>& value) {colorAttachments = value; return *this;}
        FramebufferDesc& setDepthAttachment(FramebufferAttachment value) {depthAttachment = value; return *this;}
    };
    class IFramebuffer {

    };
    struct UniformBufferBinding {
        IBuffer* buffer = nullptr;
        uint32_t set = 0, binding = 0;
        
        UniformBufferBinding& setBuffer(IBuffer* value) {buffer = value; return *this;}
        UniformBufferBinding& setSet(uint32_t value) {set = value; return *this;}
        UniformBufferBinding& setBinding(uint32_t value) {binding = value; return *this;}
    };
    struct TextureBinding {
        ISampler* sampler = nullptr;
        ITexture* texture = nullptr;
        uint32_t set = 0, binding = 0;

        TextureBinding& setTexture(ITexture* value) {texture = value; return *this;}
        TextureBinding& setSampler(ISampler* value) {sampler = value; return *this;}
        TextureBinding& setSet(uint32_t value) {set = value; return *this;}
        TextureBinding& setBinding(uint32_t value) {binding = value; return *this;}
    };
    struct ShaderResources {
        std::vector<UniformBufferBinding> uboBinding;
        std::vector<TextureBinding> texBinding;

        ShaderResources& setUboBindings(const std::vector<UniformBufferBinding> value) {uboBinding = value; return *this;}
        ShaderResources& setTexBindings(const std::vector<TextureBinding> value)       {texBinding = value; return *this;}
    };
    enum class RenderPassLoadOp {
        LOAD, CLEAR
    };
    struct RenderPassDesc {
        IFramebuffer* fb = nullptr;
        std::array<RenderPassLoadOp, 8> loadOps;
        std::array<std::array<float, 4>, 8> clearColors;
        RenderPassDesc& setFramebuffer(IFramebuffer* value) {fb = value; return *this;}
        RenderPassDesc& setAttachmentsLoadOp(const std::array<RenderPassLoadOp, 8>& value) {loadOps = value; return *this;}
        RenderPassDesc& setAttachmentsClearColors(const std::array<std::array<float, 4>, 8>& value) {clearColors = value; return *this;}
    
    };
    enum class SubPassAttachment {
        NONE,
        PRESERVE,
        INPUT,
        COLOR,
    };
    struct SubPassDesc {
        std::array<SubPassAttachment, 8> attachments;

        SubPassDesc& setAttachments(const std::array<SubPassAttachment, 8>& value) {attachments = value; return *this;}
    };
    class ICommandList {
    public:
        virtual void begin() = 0;
        virtual void bindGraphicsPipeline(IGraphicsPipeline* pipeline) = 0;
        virtual void bindShaderResources(const ShaderResources& shaderResouces) = 0;
        virtual void bindVertexBuffers(const std::vector<BindVertexBuffer>& buffers) = 0;
        virtual void bindIndexBuffer(IBuffer* buffer) = 0;
        virtual void draw(const DrawArguments& drawArgs) = 0;
        virtual void drawIndexed(const DrawArguments& drawArgs) = 0;
        virtual void writeBuffer(IBuffer* buffer, const void* data, size_t size, size_t offset) = 0;
        virtual void clearBuffer(IBuffer* buffer, uint32_t clearValue) = 0;
        //TODO: add offsets and portions of the texture
        virtual void blitTexture(ITexture* src, ITexture* dest) = 0;
        virtual void generateMipMaps(ITexture* tex) = 0;
        //TODO: add offsets and portions of the texture
        virtual void writeTexture(ITexture* tex, const void* data, TextureFormat dataFormat) = 0;
        virtual void beginRenderPass(const RenderPassDesc& desc) = 0;
        virtual void beginSubPass(const SubPassDesc& desc) = 0;
        virtual void endRenderPass() = 0;
        virtual void end() = 0;
    };
    class IDevice {
    public:
        virtual bool beginFrame() = 0;
        virtual void endFrame() = 0;
        virtual IBuffer* createBuffer(const BufferDesc& desc) = 0;
        virtual ICommandList* createCommandList() = 0;
        virtual IInputLayout* createInputLayout(const VertexAttributeDesc* attrs, size_t size) = 0;
        inline IInputLayout* createInputLayout(const std::vector<VertexAttributeDesc>& attrs) {return createInputLayout(attrs.data(), attrs.size());}
        //Raw Shader data, for opengl -> glsl text, for vulkan -> spirv
        virtual IShader* createShader(ShaderType type, const void* data, size_t size) = 0;
        virtual IGraphicsPipeline* createGraphicsPipeline(const GraphicsPipelineDesc& desc) = 0;
        virtual ITexture* createTexture(const TextureDesc& desc) = 0;
        virtual ISampler* createSampler(const SamplerDesc& desc) = 0;
        virtual IFramebuffer* createFramebuffer(const FramebufferDesc& desc) = 0;
        virtual void execute(ICommandList* cmd) = 0;
    };
}