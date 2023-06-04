#ifndef TEXTURE_SAMPLER
#define TEXTURE_SAMPLER

#include "SDK/OpenGL/CppOpenGL.h"

class TextureSampler
{
public:
    GLenum magFilter = GL_NEAREST;
    GLenum minFilter = GL_NEAREST;
    GLenum wrapS = GL_REPEAT;
    GLenum wrapT = GL_REPEAT;
};



#endif