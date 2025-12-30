#pragma once
#include <alina/alina.hpp>

namespace alina::opengl {
    class GlDevice;
    class GlShader : public IShader {
    public:
        static std::shared_ptr<IShader> createShader(GlDevice* device, ShaderType type, const void* data, size_t size, const std::string& entry_point);
        ShaderType getType() override;
        IDevice* getDevice() override;
        std::string getName() override;
        void setName(const std::string& name) override;
        
        ShaderType type;
        uint32_t id;
        GlDevice* device;
    private:
        GlShader(GlDevice* device, uint32_t id, ShaderType type);
    };
}