#pragma once
#include "alina/alina.hpp"

namespace alina::opengl {
    class InputLayout : public IInputLayout {
    public:
        InputLayout(const VertexAttributeDesc* attrs, size_t size);
        VertexAttributeDesc* getAttributes();
        size_t getNumAttributes();
        
        std::vector<VertexAttributeDesc> attrs;
    };
}