#ifndef TEXTURE_SAMPLER
#define TEXTURE_SAMPLER

#include "SDK/OpenGL/CppOpenGL.h"

class TextureSampler
{
public:
    
    GLenum wrapS = GL_REPEAT;
    GLenum wrapT = GL_REPEAT;
    GLenum magFilter = GL_NEAREST;
    GLenum minFilter = GL_NEAREST;
    TextureSampler() {}
    TextureSampler(GLenum _wrapS, GLenum _wrapT, GLenum _magFilter, GLenum _minFilter) : wrapS(_wrapS), wrapT(_wrapT), magFilter(_magFilter), minFilter(_minFilter){}
};



#endif