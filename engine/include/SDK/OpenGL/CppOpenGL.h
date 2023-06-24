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
	void ClearColor(float x, float y, float z, float w)
	{
		glClearColor(x, y, z, w);
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
class GLTexture
{
public:
	unsigned int id = -1;
	virtual GLenum GetTarget() = 0;
	GLTexture()
	{
		glGenTextures(1, &id);		
	}
	virtual ~GLTexture()
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
			glBindTexture(GetTarget(), id);
			glContext.textureID = id;
		}
	}
	void WrapST(GLint wrap_s, GLint wrap_t)
	{
		Bind();
		glTexParameteri(GetTarget(), GL_TEXTURE_WRAP_S, wrap_s);
		glTexParameteri(GetTarget(), GL_TEXTURE_WRAP_T, wrap_t);
	}
	void WrapSTR(GLint wrap_s, GLint wrap_t, GLint wrap_r)
	{
		Bind();
		glTexParameteri(GetTarget(), GL_TEXTURE_WRAP_S, wrap_s);
		glTexParameteri(GetTarget(), GL_TEXTURE_WRAP_T, wrap_t);
		glTexParameteri(GetTarget(), GL_TEXTURE_WRAP_R, wrap_t);
	}
	void Filters(GLint min_filter, GLint mag_filter)
	{
		Bind();
		glTexParameteri(GetTarget(), GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(GetTarget(), GL_TEXTURE_MAG_FILTER, mag_filter);
	}
	void Image2D(GLenum target, GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
	{
		Bind();
		glTexImage2D(target,0,internalformat,width,height,0,format,type,pixels);
	}
	void Image2D(GLenum target, int width, int height, int channel, unsigned char* data = nullptr, float* hdrData = nullptr, bool isGenMipmap = true)
    {
        GLenum format = GL_RGBA;
        if (channel == 1)
        {
            format = GL_RED;
        }
        if (channel == 3)
        {
            format = GL_RGB;
        }
        if (data)
        {
            Image2D(target, format, width, height, format, GL_UNSIGNED_BYTE, data);
        }
        else if (hdrData)
        {
            Image2D(target, format, width, height, format, GL_FLOAT, hdrData);
        }
        else
        {
            LOG("image datas is null !")
        }
        if (isGenMipmap)
        {
            GenMipmap();
        }
    }
	void Image2D(int width, int height, int channel, unsigned char* data = nullptr, float* hdrData = nullptr, bool isGenMipmap = true)
	{
		Image2D(GetTarget(), width, height, channel, data, hdrData, isGenMipmap);
	}
	void GenMipmap()
	{
		Bind();
		glGenerateMipmap(GetTarget());
	}
	void ActiveTextureUnit(GLenum index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		Bind();
	}
};
class GLTexture2D : public GLTexture
{
public:
	virtual GLenum GetTarget() override
	{
		return GL_TEXTURE_2D;
	}
};
class GLTextureCube : public GLTexture
{
public:
	virtual GLenum GetTarget() override
	{
		return GL_TEXTURE_CUBE_MAP;
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
	virtual GLenum GetTarget() = 0;
	virtual void Bind() = 0;
	void Data(GLsizeiptr size, const void *data)
	{
		Bind();
		glBufferData(GetTarget(), size, data, GL_STATIC_DRAW);
	}
	void SubData(GLintptr offset, GLsizeiptr size, const void *data)
	{
		Bind();
		glBufferSubData(GetTarget(), offset, size, data);
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
	virtual GLenum GetTarget() override
	{
		return GL_ARRAY_BUFFER;
	}
	virtual void Bind() override
	{
		if (glContext.arrayBufferID != id)
		{
			glBindBuffer(GetTarget(), id);
			glContext.arrayBufferID = id;
		}
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
	virtual GLenum GetTarget() override
	{
		return GL_ELEMENT_ARRAY_BUFFER;
	}
	virtual void Bind() override
	{
		if (glContext.elementArrayBufferID != id)
		{
			glBindBuffer(GetTarget(), id);
			glContext.elementArrayBufferID = id;
		}
	}
};
class GLPrimitive
{
public:
	GLVertexArray* vao = nullptr;
	GLArrayBuffer* vbo = nullptr;
	GLElementArrayBuffer* ebo = nullptr;
	GLPrimitive()
	{
		vao = new GLVertexArray;
		vbo = new GLArrayBuffer;
		ebo = new GLElementArrayBuffer;
	}
	~GLPrimitive()
	{
		Free();
	}
	void Free()
	{
		if (!vao)
		{
			delete vao;
			vao = nullptr;
		}
		if (!vbo)
		{
			delete vbo;
			vbo = nullptr;
		}
		if (!ebo)
		{
			delete ebo;
			ebo = nullptr;
		}
	}
	void DrawElements(GLsizei count)
	{
		vao->Bind();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	}
	template<typename T>
	void VertexAttributeData(GLuint &index, GLintptr &pointer, T&& t)
	{
		if (t.size())
		{
			vbo->SubData(pointer, sizeof(t[0])*t.size(), &t[0]);
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, sizeof(t[0])/sizeof(t[0][0]), GL_FLOAT, GL_FALSE, sizeof(t[0]), (void*)pointer);
			pointer += sizeof(t[0])*t.size();
		}
		index++;
	}
	template<typename T, typename... Args>
	void VertexAttributeData(GLuint& index, GLintptr& pointer, T&& t, Args&&... args)
	{
		VertexAttributeData(index, pointer, t);
		VertexAttributeData(index, pointer, std::forward<Args>(args)...);
	}
	template<typename... Args>
	void VertexAttributeData(Args&&... args)
	{
		vao->Bind();

		int size = 0; // GLsizeiptr
		VectorSizeof(size, std::forward<Args>(args)...);
		vbo->Data(size, NULL);

		GLuint index = 0;
		GLintptr pointer = 0;
		VertexAttributeData(index, pointer, std::forward<Args>(args)...);
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

#endif