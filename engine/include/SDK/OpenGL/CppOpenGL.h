#ifndef CPP_OPEN_GL_H
#define CPP_OPEN_GL_H

#include <glad/glad.h>
#include "Core/Log/Log.h"
#include "SDK/STL/STL.h"

struct GLShader
{
public:
	unsigned int id=-1;
	GLShader(GLenum t)
	{
		id = glCreateShader(t);
		LOG("create gl shader");
	}
	~GLShader()
	{
		glDeleteShader(id);
		LOG("delete gl shader");
	}
	bool Compile(const char *code)
	{
		glShaderSource(id, 1, &code, NULL);
		glCompileShader(id);
		int succ;
		char info[1024];
		glGetShaderiv(id, GL_COMPILE_STATUS, &succ);
		if (!succ)
		{
			glGetShaderInfoLog(id, 1024, NULL, info);
			LOG("compile shader failed ! : " << info << " !")
			return false;
		}
		LOG("compile shader succeed")
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
	bool Link(unsigned int a, unsigned int b)
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
			LOG("link GLShader failed : " << info << " !")
			return false;
		}
		LOG("link program succeed")
		return true;
	}
};
enum GLTexParam
{
	REPEAT = GL_REPEAT,
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
	NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
	RED = GL_RED,
	RGB = GL_RGB,
	RGBA = GL_RGBA,
	UBYTE = GL_UNSIGNED_BYTE

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
		glBindTexture(GL_TEXTURE_2D, id);
	}
	void SetupWrapST(GLint wrap_s, GLint wrap_t)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	}
	void SetupFilter(GLint min_filter, GLint mag_filter)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	}
	void SetupPixels(GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
	{
		glTexImage2D(GL_TEXTURE_2D,0,internalformat,width,height,0,format,type,pixels);
	}
	void GenMipmap()
	{
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
enum GLRenderBufferParam
{
	Depth24Stencil8 = GL_DEPTH24_STENCIL8
};
enum GLFrameBufferParam
{
	ColorAttachment0 = GL_COLOR_ATTACHMENT0,
	DepthStencilAttachment = GL_DEPTH_STENCIL_ATTACHMENT
};
struct GLFrameBuffer : GLBuffer
{
	void Bind()
	{
		glBindBuffer(GL_FRAMEBUFFER, id);
	}
	bool Check()
	{
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}
	void SetAttachmentTexture2D(GLenum attachment, GLuint textureId)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, textureId, 0);
	}
	void SetRenderbuffer(GLenum attachment, GLenum renderbuffer)
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
	void SetStorage(GLenum internalformat, GLsizei width, GLsizei height)
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

void GLClear()
{
	glEnable(GL_DEPTH_TEST);  
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
}
void GLLineMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
#endif