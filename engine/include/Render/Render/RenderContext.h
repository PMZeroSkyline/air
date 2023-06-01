#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "SDK/OpenGL/CppOpenGL.h"

class RenderContext
{
public:

    bool blend = false;
    bool depthTest = true;
    bool depthMask = true;
    bool cullFace = true;
    GLPolygonMode frontAndBackFaceMode = GLPolygonMode::FILL;

    void Reset()
    {
        GLBlend(blend);
        GLDepthTest(depthTest);
        GLDepthMask(depthMask);
        GLCullFace(cullFace);
        GLPolygon(frontAndBackFaceMode);

        GLBlendFunc(GLBlendFactor::SRC_ALPHA, GLBlendFactor::ONE_MINUS_SRC_ALPHA);
    }
};
RenderContext renderContext;


#endif