#ifndef CPP_OPEN_GL_H
#define CPP_OPEN_GL_H

#include <glad/glad.h>
#include "Core/Log/Log.h"
#include "SDK/STL/STL.h"
#include "Core/Operator/Sizeof.h"

#define FORCE_BIND


class GLContext
{
public:

    bool blend = false;
	GLenum sBlendFactor = GL_ONE;
	GLenum dBlendFactor = GL_ZERO;
    bool depthTest = false;
    bool depthMask = false;
    bool cullFace = false;
    GLenum frontAndBackFacePolygonMode = GL_FILL;

	unsigned int textureID = -1;
	unsigned int vertexArrayID = -1;
	unsigned int arrayBufferID = -1;
	unsigned int elementArrayBufferID = -1;
	unsigned int frameBufferID = -1;
	unsigned int renderBufferID = -1;

	void BindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		frameBufferID = 0;
	}
	void ClearColor(vec4 color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
	}
	void Clear(GLbitfield mask)
	{
		glClear(mask);  
	}
	void SetCullFace(bool isEnable)
	{
		if (cullFace != isEnable)
		{
			if (isEnable)
			{
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
			cullFace = isEnable;
		}
	}
	void SetBlend(bool isEnable)
	{
		if (blend != isEnable)
		{
			if (isEnable)
			{
				glEnable(GL_BLEND);
			}
			else
			{
				glDisable(GL_BLEND);
			}
			blend = isEnable;
		}
	}
	void SetPolygon(GLenum mode)
	{
		if (frontAndBackFacePolygonMode != mode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, mode);
			frontAndBackFacePolygonMode = mode;
		}
	}
	void SetDepthTest(bool isEnable)
	{
		if (depthTest != isEnable)
		{
			if (isEnable)
			{
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
			depthTest = isEnable;
		}
	}
	void SetDepthMask(bool isEnable)
	{
		if (depthMask != isEnable)
		{
			glDepthMask(isEnable);
			depthMask = isEnable;
		}
	}
	void SetBlendFunc(GLenum sfactor, GLenum dfactor)
	{
		if (sBlendFactor != sfactor || dBlendFactor != dfactor)
		{
			glBlendFunc(sfactor, dfactor);
			sBlendFactor = sfactor;
			dBlendFactor = dfactor;
		}
	}
};
GLContext glContext;

class GLShader
{
public:
	unsigned int id=-1;
	GLShader(GLenum t)
	{
		id = glCreateShader(t);
	}
	virtual ~GLShader()
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
class GLProgram
{
public:
	unsigned int id=-1;
	GLProgram()
	{
		id = glCreateProgram();
	}
	virtual ~GLProgram()
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
class GLTexture2D
{
public:
	unsigned int id = -1;
	GLTexture2D()
	{
		glGenTextures(1, &id);		
	}
	virtual ~GLTexture2D()
	{
		glDeleteTextures(1, &id);
		if (glContext.textureID == id)
		{
			glContext.textureID = -1;
		}
	}
	void Bind()
	{
		if (glContext.textureID != id)
		{
			glBindTexture(GL_TEXTURE_2D, id);
			glContext.textureID = id;
		}
	}
	void WrapST(GLint wrap_s, GLint wrap_t)
	{
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	}
	void Filters(GLint min_filter, GLint mag_filter)
	{
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	}
	void Image2D(GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
	{
		Bind();
		glTexImage2D(GL_TEXTURE_2D,0,internalformat,width,height,0,format,type,pixels);
	}
	void GenMipmap()
	{
		Bind();
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void Active(GLenum index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		Bind();
	}
	void BorderColor(vec4 borderColor)
	{
		Bind();
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &(borderColor[0]));
	}
};
class GLVertexArray
{
public:
	unsigned int id;
	GLVertexArray()
	{
		glGenVertexArrays(1, &id);
	}
	virtual ~GLVertexArray()
	{
		glDeleteVertexArrays(1, &id);
		if (glContext.vertexArrayID == id)
		{
			glContext.vertexArrayID = -1;
		}
	}
	void Bind()
	{
		if (glContext.vertexArrayID != id)
		{
			glBindVertexArray(id);
			glContext.vertexArrayID = id;
		}
	}
};
class GLBuffer
{
public:
	unsigned int id;
	GLBuffer()
	{
		glGenBuffers(1, &id);
	}
	virtual ~GLBuffer()
	{
		glDeleteBuffers(1, &id);
	}
};
class GLArrayBuffer : public GLBuffer
{
public:
	 ~GLArrayBuffer()
	 {
	 	if (glContext.arrayBufferID == id)
	 	{
	 		glContext.arrayBufferID = -1;
	 	}
	 }
	void Bind()
	{
		if (glContext.arrayBufferID != id)
		{
			glBindBuffer(GL_ARRAY_BUFFER, id);
			glContext.arrayBufferID = id;
		}
	}
};
 class GLUniformBuffer : GLBuffer
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
class GLElementArrayBuffer : public GLBuffer
{
public:
	 ~GLElementArrayBuffer()
	 {
	 	if (glContext.elementArrayBufferID == id)
	 	{
	 		glContext.elementArrayBufferID = -1;
	 	}
	 }
	void Bind()
	{
		if (glContext.elementArrayBufferID != id)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
			glContext.elementArrayBufferID = id;
		}
	}
};
class GLFrameBuffer
{
public:
	unsigned int id;
	GLFrameBuffer()
	{
		glGenFramebuffers(1, &id);
	}
	~GLFrameBuffer()
	{
		glDeleteFramebuffers(1, &id);
		if (glContext.frameBufferID == id)
		{
			glContext.frameBufferID = -1;
		}
	}
	void Bind()
	{
		if (glContext.frameBufferID != id)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, id);
			glContext.frameBufferID = id;
		}
	}
	bool IsComplete()
	{
		Bind();
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}
	void Texture2D(GLenum attachment, GLuint textureId)
	{
		Bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, textureId, 0);
	}
	void Renderbuffer(GLenum attachment, GLuint renderbuffer)
	{
		Bind();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer);
	}
	void DrawBuffers(vector<unsigned int>& attachments)
	{
		Bind();
		glDrawBuffers(attachments.size(), &(attachments[0]));
	}
	void DrawBuffer(GLenum buf)
	{
		Bind();
		glDrawBuffer(buf);
	}
	void ReadBuffer(GLenum src)
    {
		Bind();
		glReadBuffer(src);
	}
};
class GLRenderBuffer
{
public:
	unsigned int id;
	GLRenderBuffer()
	{
		glGenRenderbuffers(1, &id);
	}
	~GLRenderBuffer()
	{
		glDeleteRenderbuffers(1, &id);
		if (glContext.renderBufferID == id)
		{
			glContext.renderBufferID = -1;
		}
	}
	void Bind()
	{
		if (glContext.renderBufferID != id)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, id);
			glContext.renderBufferID = id;
		}
	}
	void SetStorage(GLenum internalformat, GLsizei width, GLsizei height)
	{
		Bind();
		glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
	}
};
class GLPrimitive
{
public:
	GLArrayBuffer Vbo;
	GLVertexArray Vao;
	GLElementArrayBuffer Ebo;
	void Bind()
	{
		Vao.Bind();
		Vbo.Bind();
		Ebo.Bind();
	}
	void DrawElements(GLsizei count)
	{
		Bind();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	}
	template<typename T>
	void VaoData(int &target, int &offset, T&& t)
	{
		if (t.size())
		{
			glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(t[0])*t.size(), &t[0]);
			glEnableVertexAttribArray(target);
			GLenum type = GL_FLOAT;
			if (typeid(t[0][0]) == typeid(float))
			{
				type = GL_FLOAT;
			}
			else if (typeid(t[0][0]) == typeid(int))
			{
				type = GL_INT;
			}
			else if (typeid(t[0][0]) == typeid(unsigned int))
			{
				type = GL_UNSIGNED_INT;
			}
			else if (typeid(t[0][0]) == typeid(unsigned char))
			{
				type = GL_UNSIGNED_BYTE;
			}
			else
			{
				LOG("failed to find vao data type !")
			}
			long long offset64bit = offset;
			glVertexAttribPointer(target, sizeof(t[0])/sizeof(t[0][0]), type, GL_FALSE, sizeof(t[0]), (void*)offset64bit);
			offset += sizeof(t[0])*t.size();
		}
		target++;
	}
	template<typename T, typename... Args>
	void VaoData(int &target, int &offset, T&& t, Args&&... args)
	{
		VaoData(target, offset, t);
		VaoData(target, offset, std::forward<Args>(args)...);
	}
	template<typename... Args>
	void VaoData(Args&&... args)
	{
		Bind();
		int size = 0, offset = 0, target = 0;
		VectorSizeof(size, std::forward<Args>(args)...);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
		VaoData(target, offset, std::forward<Args>(args)...);
	}
	void EboData(const vector<unsigned int> &ids)
	{
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ids[0])*ids.size(), &ids[0], GL_STATIC_DRAW);
	}
};

#endif