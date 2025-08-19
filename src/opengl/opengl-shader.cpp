#include <alina/opengl-shader.hpp>
#include <alina/opengl-device.hpp>
#include <alina/opengl-conversions.hpp>
#include <iostream>
#include <memory>

namespace alina::opengl {
    std::shared_ptr<IShader> GlShader::createShader(GlDevice* device, ShaderType type, const void* data, size_t size) {
        GladGLContext context = device->context;
        uint32_t id = context.CreateShader(shaderTypeToGl(type));
        const char* str = static_cast<const char*>(data);
        context.ShaderSource(id, 1, &str, reinterpret_cast<const int*>(&size));        
        context.CompileShader(id);

        GLint ok;
        context.GetShaderiv(id, GL_COMPILE_STATUS, &ok);
        if (ok == GL_FALSE)
        {
            GLsizei log_length = 0;
            GLchar message[1024];
            context.GetShaderInfoLog(id, 1024, NULL, message);
            std::cout << "ERROR COMPILING SHADER " << message << "\n";
            //TODO: write to a log
            return nullptr;
        }
        return std::shared_ptr<GlShader>(new GlShader(device, id, type));
    }
    GlShader::GlShader(GlDevice* device, uint32_t id, ShaderType type) {
        this->id = id;
        this->type = type;
        this->device = device;
    }
    ShaderType GlShader::getType() {
        return type;
    }
    IDevice* GlShader::getDevice() {return device;}
    std::string GlShader::getName() {
        GLsizei length;
        device->context.GetObjectLabel(GL_BUFFER, id, 0, &length, nullptr);
        std::string str;
        str.resize(length, '\0');
        device->context.GetObjectLabel(GL_BUFFER, id, length+1, &length, str.data());
        return str;
    }
    void GlShader::setName(const std::string& name) {
        device->context.ObjectLabel(GL_BUFFER, id, name.size(), name.data());
    }
}