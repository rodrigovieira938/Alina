#pragma once
#include <alina/alina.hpp>
#include <alina/opengl-device.hpp>

namespace alina::opengl {
    class GlInputLayout : public IInputLayout {
    public:
        GlInputLayout(GlDevice* device, const VertexAttributeDesc* attrs, size_t size);
        VertexAttributeDesc* getAttributes() override;
        size_t getNumAttributes() override;
        IDevice* getDevice() override {return device;}
        std::string getName() override {return mName;}
        void setName(const std::string& name) override {mName = name;}

        std::string mName;
        std::vector<VertexAttributeDesc> attrs;
        GlDevice* device;
    };
}