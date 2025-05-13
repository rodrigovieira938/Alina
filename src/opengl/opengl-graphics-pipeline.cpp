#include <alina/alina.hpp>
#include <alina/opengl-graphics-pipeline.hpp>
#include <alina/opengl-conversions.hpp>
#include <alina/opengl-device.hpp>
namespace alina::opengl {
    static InputLayout* defaultInputLayout = nullptr;
    GraphicsPipeline::GraphicsPipeline(GraphicsPipelineDesc desc, Device* device) {
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
            context.VertexArrayAttribFormat(vertexArray, i, attr.arraySize, vertexAttrFormatToGl(attr.format), GL_FALSE, attr.offset);
            context.VertexArrayAttribBinding(vertexArray, i, attr.bufferIndex);
            context.VertexAttribDivisor(i, attr.isInstanced ? 1 : 0);
        }
    }
    void GraphicsPipeline::bind() {
        device->context.CullFace(rasterCullModeToGl(desc.renderState.rasterState.cullMode));
        device->context.PolygonMode(GL_FRONT_AND_BACK, rasterFillModeToGl(desc.renderState.rasterState.fillMode));
        device->context.FrontFace(desc.renderState.rasterState.frontCounterClockwise ? GL_CW : GL_CCW);
        device->context.BindVertexArray(vertexArray);
    }
}