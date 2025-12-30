#include "glad/gl.h"
#include <algorithm>
#include <alina/opengl-shader.hpp>
#include <alina/opengl-device.hpp>
#include <alina/opengl-conversions.hpp>
#include <iostream>
#include <memory>
#include <string_view>

namespace alina::opengl {
    uint32_t checkCompileStatus(GladGLContext context, uint32_t id) {
        GLint ok;
        context.GetShaderiv(id, GL_COMPILE_STATUS, &ok);
        if (ok == GL_FALSE)
        {
            GLsizei log_length = 0;
            GLchar message[1024];
            context.GetShaderInfoLog(id, 1024, NULL, message);
            std::cout << "ERROR COMPILING SHADER " << message << "\n";
            //TODO: write to a log
            context.DeleteShader(id);
            return 0;
        }
        return id;
    }
    uint32_t createGLSLShader(GladGLContext context, ShaderType type, const void* data, size_t size) {
        uint32_t id = context.CreateShader(shaderTypeToGl(type));
        const char* str = static_cast<const char*>(data);
        context.ShaderSource(id, 1, &str, reinterpret_cast<const int*>(&size));        
        context.CompileShader(id);
        return checkCompileStatus(context, id);
    }
    uint32_t createSPIRVShader(GladGLContext context, ShaderType type, const void* data, size_t size, const std::string& entry_point) {
        uint32_t id = context.CreateShader(shaderTypeToGl(type));
        context.ShaderBinary(1, &id, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, data, size);
        context.SpecializeShaderARB(id, entry_point.data(), 0, 0, 0);
        return checkCompileStatus(context, id);
    }
    std::shared_ptr<IShader> GlShader::createShader(GlDevice* device, ShaderType type, const void* data, size_t size, const std::string& entry_point) {
        static auto SPIRV_MAGIC_NUMBER_LE = "\x03\x02\x23\x07";
        static auto SPIRV_MAGIC_NUMBER_BE = "\x07\x23\x02\x03";
        uint32_t id;
        std::string_view str((char*)data, std::min<size_t>(size, 4));
        if(str == SPIRV_MAGIC_NUMBER_LE || str == SPIRV_MAGIC_NUMBER_BE) {
            id = createSPIRVShader(device->context, type, data, size, entry_point);
        } else {
            id = createGLSLShader(device->context, type, data, size);        
        }
        if(id == 0)
            return nullptr;
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