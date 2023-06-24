#ifndef TEXTURE_SAMPLER
#define TEXTURE_SAMPLER

#include "SDK/OpenGL/CppOpenGL.h"

class TextureSampler
{
public:
    GLenum minFilter = GL_NEAREST;
    GLenum magFilter = GL_NEAREST;

    GLenum wrapS = GL_REPEAT;
    GLenum wrapT = GL_REPEAT;
    GLenum wrapR = GL_REPEAT;    
    
    TextureSampler(){}
    TextureSampler(GLenum _minFilter, GLenum _magFilter, GLenum _wrapS, GLenum _wrapT, GLenum _wrapR = GL_REPEAT) : minFilter(_minFilter), magFilter(_magFilter), wrapS(_wrapS), wrapT(_wrapT), wrapR(_wrapR) {}
    
};



#endif