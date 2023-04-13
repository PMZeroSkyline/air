#ifndef MESH_COMMIT_H
#define MESH_COMMIT_H

#include "SDK/OpenGL/CppOpenGL.h"
#include "SDK/STL/STL.h"
#include "Core/Operator/Sizeof.h"

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
		long long offset64bit = offset;
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