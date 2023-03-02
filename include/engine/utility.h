#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>
#include <iostream>
using std::string;
using std::vector;

// 以下IO函数使用了快速读取方法 https://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
void string_from_file(const string &path, string &contents)
{
	FILE *fp = fopen(path.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		contents.resize(ftell(fp));
		rewind(fp);
		fread(&contents[0], 1, contents.size(), fp);
		fclose(fp);
		return;
	}
	throw(errno);
}
template<typename T>
void vector_from_file(const string &path, int beg, int count, vector<T> &contents)
{
	FILE *fp = fopen(path.c_str(), "rb");
	if (fp)
	{
		fseek(fp, beg, SEEK_SET);
		contents.resize(count);
		fread(&contents[0], sizeof(T), count, fp);
		fclose(fp);
		return;
	}
	throw(errno);
}
template<typename T>
void get_vectors_size(int &size, const T &t)
{
	size += sizeof(t[0]) * t.size();
}
template<typename T, typename... Args>
void get_vectors_size(int &size, const T &t, const Args&... args)
{
	get_vectors_size(size, t);
	get_vectors_size(size, args...);
}
#endif