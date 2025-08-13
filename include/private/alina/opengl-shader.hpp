#pragma once
#include <alina/alina.hpp>

namespace alina::opengl {
    class GlDevice;
    class GlShader : public IShader {
    public:
        static std::shared_ptr<IShader> createShader(GlDevice* device, ShaderType type, const void* data, size_t size);
        ShaderType getType() override;

        ShaderType type;
        uint32_t id;
    private:
        GlShader(uint32_t id, ShaderType type);
    };
}