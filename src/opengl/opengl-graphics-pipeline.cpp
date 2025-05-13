#pragma once
#include <alina/opengl-graphics-pipeline.hpp>
#include <alina/opengl-conversions.hpp>
#include <alina/opengl-device.hpp>
namespace alina::opengl {
    GraphicsPipeline::GraphicsPipeline(GraphicsPipelineDesc desc, Device* device) {
        this->desc = desc;
        this->device = device;
    }
    void GraphicsPipeline::bind() {
        device->context.CullFace(rasterCullModeToGl(desc.renderState.rasterState.cullMode));
        device->context.PolygonMode(GL_FRONT_AND_BACK, rasterFillModeToGl(desc.renderState.rasterState.fillMode));
        device->context.FrontFace(desc.renderState.rasterState.frontCounterClockwise ? GL_CW : GL_CCW);
    }
}