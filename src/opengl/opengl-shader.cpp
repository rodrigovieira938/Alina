#include <alina/opengl-shader.hpp>
#include <alina/opengl-device.hpp>
#include <alina/opengl-conversions.hpp>
#include <iostream>

namespace alina::opengl {
    Shader* Shader::createShader(Device* device, ShaderType type, const void* data, size_t size) {
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
        return new Shader(id, type);
    }
    Shader::Shader(uint32_t id, ShaderType type) {
        this->id = id;
        this->type = type;
    }
    ShaderType Shader::getType() {
        return type;
    }
}