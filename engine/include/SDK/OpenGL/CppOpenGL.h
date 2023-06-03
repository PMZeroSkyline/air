#ifndef CPP_OPEN_GL_H
#define CPP_OPEN_GL_H

#include <glad/glad.h>
#include "Core/Log/Log.h"
#include "SDK/STL/STL.h"

enum GLAttachment
{
	COLOR_ATTACHMENT0 = GL_COLOR_ATTACHMENT0,
	DEPTH_STENCIL_ATTACHMENT = GL_DEPTH_STENCIL_ATTACHMENT,
	DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT
};
enum GLBlendFactor
{
    ZERO = GL_ZERO,
    ONE = GL_ONE,
    SRC_COLOR = GL_SRC_COLOR,
    ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
    DST_COLOR = GL_DST_COLOR,
    ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
    SRC_ALPHA = GL_SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
    DST_ALPHA = GL_DST_ALPHA,
    ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
    CONSTANT_COLOR = GL_CONSTANT_COLOR,
    ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
    CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
    ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA
};
enum GLPolygonMode
{
	LINE = GL_LINE,
	FILL = GL_FILL
};
enum GLFormat
{
	RED = GL_RED,
	RGB = GL_RGB,
	RGBA = GL_RGBA,
	DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
	DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8
};
enum GLType
{
	UBYTE = GL_UNSIGNED_BYTE,
	FLOAT = GL_FLOAT,
};
enum GLMask
{
	COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT,
	DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT,
	STENCIL_BUFFER_BIT = GL_STENCIL_BUFFER_BIT
};
enum GLFilter
{
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
	NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
};
enum GLWrap
{
	REPEAT = GL_REPEAT,
	CLAMP =	GL_CLAMP
};
enum GLCullMode
{
    FRONT = GL_FRONT,
    BACK = GL_BACK,
    FRONT_AND_BACK = GL_FRONT_AND_BACK
};
class RenderContext
{
public:

    bool blend = false;
    bool depthTest = true;
    bool depthMask = true;
    bool cullFace = true;
    GLPolygonMode frontAndBackFaceMode = GLPolygonMode::FILL;

	unsigned int shader = -1;
	unsigned int texture = -1;
};
RenderContext renderContext;
struct GLShader
{
public:
	unsigned int id=-1;
	GLShader(GLenum t)
	{
		id = glCreateShader(t);
	}
	~GLShader()
	{
		glDeleteShader(id);
	}
	bool Compile(const char *code, const string& error)
	{
		glShaderSource(id, 1, &code, NULL);
		glCompileShader(id);
		int succ;
		char info[1024];
		glGetShaderiv(id, GL_COMPILE_STATUS, &succ);
		if (!succ)
		{
			glGetShaderInfoLog(id, 1024, NULL, info);
			LOG(error << info << " !")
			return false;
		}
		return true;
	}
};
struct GLProgram
{
	unsigned int id=-1;
	GLProgram()
	{
		id = glCreateProgram();
	}
	~GLProgram()
	{
		glDeleteProgram(id);
	}
	bool Link(unsigned int a, unsigned int b, const string& error)
	{
		glAttachShader(id, a);
		glAttachShader(id, b);
		glLinkProgram(id);
		int succ;
		char info[1024];
		glGetProgramiv(id, GL_LINK_STATUS, &succ);
		if (!succ)
		{
			glGetProgramInfoLog(id, 1024, NULL, info);
			LOG(error << info << " !")
			return false;
		}
		return true;
	}
};
struct GLTexture2D
{
	unsigned int id = -1;
	GLTexture2D()
	{
		glGenTextures(1, &id);		
	}
	void Bind()
	{
		if (renderContext.texture != id)
		{
			glBindTexture(GL_TEXTURE_2D, id);
			renderContext.texture = id;
		}
	}
	void WrapST(GLWrap wrap_s, GLWrap wrap_t)
	{
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	}
	void Filters(GLFilter min_filter, GLFilter mag_filter)
	{
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	}
	void Image2D(GLFormat internalformat, GLsizei width, GLsizei height, GLFormat format, GLType type, const void *pixels)
	{
		Bind();
		glTexImage2D(GL_TEXTURE_2D,0,internalformat,width,height,0,format,type,pixels);
	}
	void GenMipmap()
	{
		Bind();
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	~GLTexture2D()
	{
		glDeleteTextures(1, &id);
	}
};
struct GLVertexArray
{
	unsigned int id;
	GLVertexArray()
	{
		glGenVertexArrays(1, &id);
	}
	~GLVertexArray()
	{
		glDeleteVertexArrays(1, &id);
	}
	void Bind()
	{
		glBindVertexArray(id);
	}
};
struct GLBuffer
{
	unsigned int id;
	GLBuffer()
	{
		glGenBuffers(1, &id);
	}
	~GLBuffer()
	{
		glDeleteBuffers(1, &id);
	}
};
struct GLArrayBuffer : GLBuffer
{
	void Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}
};
struct GLUniformBuffer : GLBuffer
{
	string name;
	void Bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, id);
	}
	void BindBufferBase(int index)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, index, id);
	}
};
struct GLElementArrayBuffer : GLBuffer
{
	void Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}
};
struct GLFrameBuffer
{
	unsigned int id;
	GLFrameBuffer()
	{
		glGenFramebuffers(1, &id);
	}
	~GLFrameBuffer()
	{
		glDeleteFramebuffers(1, &id);
	}
	void Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}
	bool IsComplete()
	{
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}
	void Texture2D(GLAttachment attachment, GLuint textureId)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, textureId, 0);
	}
	void SetRenderbuffer(GLAttachment attachment, GLenum renderbuffer)
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer);
	}
};
struct GLRenderBuffer
{
	unsigned int id;
	GLRenderBuffer()
	{
		glGenRenderbuffers(1, &id);
	}
	~GLRenderBuffer()
	{
		glDeleteRenderbuffers(1, &id);
	}
	void Bind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, id);
	}
	void SetStorage(GLFormat internalformat, GLsizei width, GLsizei height)
	{
		glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
	}
};
struct GLPrimitive
{
	GLArrayBuffer Vbo;
	GLVertexArray Vao;
	GLElementArrayBuffer Ebo;
	void Bind()
	{
		Vao.Bind();
		Vbo.Bind();
		Ebo.Bind();
	}
};



// Direct Call
void GLBindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void GLActiveTexture(int slotIndex)
{
	glActiveTexture(GL_TEXTURE0 + slotIndex);
}
void GLDrawElements(GLsizei count)
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}
void GLClearColor(vec4 color)
{
	glClearColor(color.x, color.y, color.z, color.w);
}
void GLClear(GLbitfield mask)
{
	glClear(mask);  
}

// RenderContext
void GLCullFace(bool isEnable)
{
	if (isEnable)
	{
		glEnable(GL_CULL_FACE);
		
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}
void GLBlend(bool isEnable)
{
	if (isEnable)
	{
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}
}
void GLPolygon(GLPolygonMode mode)
{
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}
void GLDepthTest(bool isEnable)
{
	if (isEnable)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}
void GLDepthMask(bool isEnable)
{
	glDepthMask(isEnable);
}

// Utility
void GLBlendFunc(GLBlendFactor sfactor, GLBlendFactor dfactor)
{
	glBlendFunc(sfactor, dfactor);
}

void GLResetRenderContext(RenderContext& context)
{
    GLBlend(context.blend);
    GLDepthTest(context.depthTest);
    GLDepthMask(context.depthMask);
    GLCullFace(context.cullFace);
    GLPolygon(context.frontAndBackFaceMode);
    GLBlendFunc(GLBlendFactor::SRC_ALPHA, GLBlendFactor::ONE_MINUS_SRC_ALPHA);
}
#endif