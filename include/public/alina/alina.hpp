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
        virtual void execute(CommandList* cmd) = 0;
    };
}