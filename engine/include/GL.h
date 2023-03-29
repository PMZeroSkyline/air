#ifndef GL_H
#define GL_H

#include "Core.h"

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
			LOG("compile shader failed ! : " << info)
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
			LOG("link GLShader failed : " << info)
			return false;
		}
		LOG("link program succeed")
		return true;
	}
};
struct GLTexture2D
{
	unsigned int id = -1;
	GLTexture2D(){}
	GLTexture2D(GLint wrap_s, GLint wrap_t, GLint min_filter, GLint mag_filter, GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels, bool is_gen_mipmap = true)
	{
		glGenTextures(1, &id);		
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

		if (is_gen_mipmap)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glTexImage2D(GL_TEXTURE_2D,0,internalformat,width,height,0,format,type,pixels);
	}
	~GLTexture2D()
	{
		if (id != -1)
			glDeleteTextures(1, &id);
	}
};
struct GLPrimitive
{
	unsigned int vao = -1, vbo = -1, ebo = -1;
	GLPrimitive()
	{
		glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
	}
	~GLPrimitive()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
	void Bind()
	{
		glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}
};
template<typename T>
void GLVaoData(int &target, int &offset, T&& t)
{
	if (t.size())
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(t[0])*t.size(), &t[0]);
		glEnableVertexAttribArray(target);
		GLenum type = GL_FLOAT;
		if (typeid(t[0][0]) == typeid(unsigned int))
			type = GL_UNSIGNED_INT;
		int64 offset64bit = offset;
		glVertexAttribPointer(target, sizeof(t[0])/sizeof(t[0][0]), type, GL_FALSE, sizeof(t[0]), (void*)offset64bit);
		offset += sizeof(t[0])*t.size();
	}
	target++;
}
template<typename T, typename... Args>
void GLVaoData(int &target, int &offset, T&& t, Args&&... args)
{
	GLVaoData(target, offset, t);
	GLVaoData(target, offset, std::forward<Args>(args)...);
}
template<typename... Args>
void GLVaoData(Args&&... args)
{
	int size = 0, offset = 0, target = 0;
	VectorSizeof(size, std::forward<Args>(args)...);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	GLVaoData(target, offset, std::forward<Args>(args)...);
}

void GLEboData(const vector<unsigned int> &ids)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ids[0])*ids.size(), &ids[0], GL_STATIC_DRAW);
}

#endif