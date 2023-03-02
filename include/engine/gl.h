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
template<typename... Args>
void gl_vao_data(const Args&... args)
{
	int size = 0, offset = 0, target = 0;
	get_vectors_size(size, args...);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	gl_vao_data(target, offset, args...);
}
template<typename T>
void gl_vao_data(int &target, int &offset, const T &t)
{
	if (t.size())
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(t[0])*t.size(), &t[0]);
		glEnableVertexAttribArray(target);
		GLenum type = GL_FLOAT;
		if (typeid(t[0][0]) == typeid(unsigned int))
			type = GL_UNSIGNED_INT;
		glVertexAttribPointer(target, sizeof(t[0])/sizeof(t[0][0]), type, GL_FALSE, sizeof(t[0]), (void*)(long)offset);
		offset += sizeof(t[0])*t.size();
	}
	target++;
}
template<typename T, typename... Args>
void gl_vao_data(int &target, int &offset, const T &t, const Args&... args)
{
	gl_vao_data(target, offset, t);
	gl_vao_data(target, offset, args...);
}
void gl_ebo_data(const vector<unsigned int> &ids)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ids[0])*ids.size(), &ids[0], GL_STATIC_DRAW);
}
#endif