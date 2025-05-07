#include <alina/opengl-conversions.hpp>
namespace alina::opengl {
    GLenum bufferTypeToGl(const BufferType& type) {
        switch (type) {
            case BufferType::VERTEX:
                return GL_VERTEX_ARRAY;
            case BufferType::INDEX:
                return GL_ELEMENT_ARRAY_BUFFER;
        }
        return -1;
    }
}