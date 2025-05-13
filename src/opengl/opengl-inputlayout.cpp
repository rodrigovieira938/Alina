#include <alina/opengl-inputlayout.hpp>
namespace alina::opengl {
    InputLayout::InputLayout(const VertexAttributeDesc* attrs, size_t size) : attrs(attrs, attrs+size) {

    }
    VertexAttributeDesc* InputLayout::getAttributes() {
        return attrs.data();
    }
    size_t InputLayout::getNumAttributes() {
        return attrs.size();
    }
}