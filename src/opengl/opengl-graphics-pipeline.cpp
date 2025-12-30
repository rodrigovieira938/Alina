#include <alina/alina.hpp>
#include <alina/opengl-graphics-pipeline.hpp>
#include <alina/opengl-conversions.hpp>
#include <alina/opengl-device.hpp>
#include <alina/opengl-shader.hpp>
#include <iostream>
#include <memory>
namespace alina::opengl {
    static InputLayout defaultInputLayout = nullptr;
    GlGraphicsPipeline::GlGraphicsPipeline(GraphicsPipelineDesc desc, GlDevice* device) {
        if(!defaultInputLayout)
            defaultInputLayout = device->createInputLayout(
                {
                    VertexAttributeDesc().setArraySize(3).setFormat(VertexAttributeFormat::Float).setStride(sizeof(float) * 3)
                }
            );
        desc.inputLayout = (desc.inputLayout == nullptr) ? defaultInputLayout : desc.inputLayout;  
        this->desc = desc;
        this->device = device;
        device->context.CreateVertexArrays(1, &vertexArray);
        VertexAttributeDesc* attrs = desc.inputLayout->getAttributes();
        size_t attrs_size = desc.inputLayout->getNumAttributes();

        auto context = this->device->context; 
        for(int i = 0; i < attrs_size; i++) {
            auto attr = attrs[i];
            context.EnableVertexArrayAttrib(vertexArray, i);
            context.VertexArrayAttribBinding(vertexArray, i, attr.bufferIndex);
            if(attr.isInstanced)
                context.VertexArrayBindingDivisor(vertexArray, i, 1);
            context.VertexArrayAttribFormat(vertexArray, i, attr.arraySize, vertexAttrFormatToGl(attr.format), GL_FALSE, attr.offset);
        }
        if(desc.vs != nullptr || desc.fs != nullptr) {
            this->program = device->context.CreateProgram();
            if(desc.vs)
                device->context.AttachShader(program, ((GlShader*)desc.vs.get())->id);
            if(desc.fs)
                device->context.AttachShader(program, ((GlShader*)desc.fs.get())->id);
            device->context.LinkProgram(program);
            GLint linked;
            context.GetProgramiv(program, GL_LINK_STATUS, &linked);
            if (!linked) {
                GLint logLength;
                context.GetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
                std::string log;
                log.resize(logLength);
                context.GetProgramInfoLog(program, logLength, NULL, log.data());
                std::cout << "ERROR LINKING PROGRAM " << log << "\n";
                context.DeleteProgram(program);
                program = 0;
            }
        }
    }
    void GlGraphicsPipeline::bind() {
        device->context.CullFace(rasterCullModeToGl(desc.renderState.rasterState.cullMode));
        device->context.PolygonMode(GL_FRONT_AND_BACK, rasterFillModeToGl(desc.renderState.rasterState.fillMode));
        device->context.FrontFace(desc.renderState.rasterState.frontCounterClockwise ? GL_CW : GL_CCW);
        device->context.BindVertexArray(vertexArray);
        device->context.UseProgram(program);
    }
    IDevice* GlGraphicsPipeline::getDevice() {return device;}
}