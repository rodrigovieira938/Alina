#include <alina/opengl-inputlayout.hpp>
namespace alina::opengl {
    GlInputLayout::GlInputLayout(GlDevice* device, const VertexAttributeDesc* _attrs, size_t size) {
        this->device = device;
        attrs.reserve(size);
        for(int i = 0; i < size; i++){
            auto attr = _attrs[i];
            if(attr.arraySize <= 4) {
                attrs.push_back(attr);
            } else {
                //Split the big array size into the smaller attributes (the least possible)
                uint32_t remainder = attr.arraySize;
                uint32_t typeSize = 0;
                switch (attr.format) {
                case VertexAttributeFormat::Int:
                case VertexAttributeFormat::Unsigned_Int:
                case VertexAttributeFormat::Float:
                    typeSize = 4;
                  break;
                }
                static uint32_t values[] = {4, 3, 2, 1};
                for(int x = 0; x < sizeof(values)/values[0]; x++) {
                    while (remainder >= values[x]) {
                        attrs.push_back(VertexAttributeDesc(attr).setArraySize(values[x]));
                        remainder -= values[x];
                        attr.offset += values[x] * typeSize;
                    }
                }
            }
        }
    }
    VertexAttributeDesc* GlInputLayout::getAttributes() {
        return attrs.data();
    }
    size_t GlInputLayout::getNumAttributes() {
        return attrs.size();
    }
}