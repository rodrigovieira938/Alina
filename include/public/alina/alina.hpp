#pragma once
#include <string>

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
    class Device {
    public:
        virtual bool beginFrame() = 0;
        virtual void endFrame() = 0;
        virtual Buffer* createBuffer(const BufferDesc& desc) = 0;
    };
}