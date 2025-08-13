#include <alina/opengl-inputlayout.hpp>
namespace alina::opengl {
    GlInputLayout::GlInputLayout(const VertexAttributeDesc* attrs, size_t size) : attrs(attrs, attrs+size) {

    }
    VertexAttributeDesc* GlInputLayout::getAttributes() {
        return attrs.data();
    }
    size_t GlInputLayout::getNumAttributes() {
        return attrs.size();
    }
}