#pragma once
#include <alina/alina.hpp>

namespace alina::opengl {
    typedef void (*GLfunction)(void);
    typedef GLfunction (*glLoadFunction)(const char *name);
    IDevice* CreateDevice(glLoadFunction fn);
} 