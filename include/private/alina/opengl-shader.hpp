#pragma once
#include <alina/alina.hpp>

namespace alina::opengl {
    class Device;
    class Shader : public ::alina::Shader {
    public:
        static Shader* createShader(Device* device, ShaderType type, const void* data, size_t size);
        ShaderType getType() override;

        ShaderType type;
        uint32_t id;
    private:
        Shader(uint32_t id, ShaderType type);
    };
}