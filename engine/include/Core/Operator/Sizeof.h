#ifndef SIZEOF_H
#define SIZEOF_H

#include "SDK/STL/STL.h"

template<typename T>
void VectorSizeof(int &size, T&& t)
{
	size += sizeof(t[0]) * t.size();
}
template<typename T, typename... Args>
void VectorSizeof(int &size, T&& t, Args&&... args)
{
	VectorSizeof(size, t);
	VectorSizeof(size, std::forward<Args>(args)...);
}

#endif