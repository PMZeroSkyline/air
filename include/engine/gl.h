#ifndef GL_H
#define GL_H

#include <glad/glad.h>
#include "utility.h"

struct gl_shader
{
public:
	unsigned int id=-1;
	gl_shader(GLenum t)
	{
		id = glCreateShader(t);
	}
	~gl_shader()
	{
		glDeleteShader(id);
	}
	void compile(const char *code)
	{
		glShaderSource(id, 1, &code, NULL);
		glCompileShader(id);
		int succ;
		char info[1024];
		glGetShaderiv(id, GL_COMPILE_STATUS, &succ);
		if (!succ)
		{
			glGetShaderInfoLog(id, 1024, NULL, info);
			std::cout << "compile gl_shader failed : " << info << std::endl;
		}
	}
};
struct gl_program
{
	unsigned int id=-1;
	gl_program()
	{
		id = glCreateProgram();
	}
	~gl_program()
	{
		glDeleteProgram(id);
	}
	void link(unsigned int a, unsigned int b)
	{
		glAttachShader(id, a);
		glAttachShader(id, b);
		glLinkProgram(id);
		int succ;
		char info[1024];
		glGetShaderiv(id, GL_LINK_STATUS, &succ);
		if (!succ)
		{
			glGetProgramInfoLog(id, 1024, NULL, info);
			std::cout << "link gl_shader failed : " << info << std::endl;
		}
	}
};
struct gl_texture
{
	unsigned int id = -1;
	gl_texture(GLenum target, GLint wrap_s, GLint wrap_t, GLint min_filter, GLint mag_filter, GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels, bool gen_mipmap)
	{
		glGenTextures(1, &id);		
		glBindTexture(target, id);

		glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter);

		if (gen_mipmap)
			glGenerateMipmap(target);

		glTexImage2D(target,0,internalformat,width,height,0,format,type,pixels);
	}
	~gl_texture()
	{
		glDeleteTextures(1, &id);
	}
};
struct gl_mesh
{
	unsigned int vao = -1, vbo = -1, ebo = -1;
	gl_mesh()
	{
		glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
	}
	~gl_mesh()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
	void bind()
	{
		glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}
};
#endif