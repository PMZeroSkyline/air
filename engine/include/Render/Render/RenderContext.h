#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "SDK/OpenGL/CppOpenGL.h"

class RenderContext
{
public:
    bool isCullFace = false;
    GLCullMode cullFace = GLCullMode::BACK;

    bool isBlend = false;
    GLBlendFactor sfactor = GLBlendFactor::SRC_ALPHA;
    GLBlendFactor dfactor = GLBlendFactor::ONE_MINUS_SRC_ALPHA;

    GLFaceMode frontAndBackFaceMode = GLFaceMode::FILL;

    bool depthTest = false;
};
RenderContext renderContext;


#endif