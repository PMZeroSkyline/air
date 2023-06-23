#ifndef BIN_FILE_READ_H
#define BIN_FILE_READ_H

#include "Core/Log/Log.h"
#include "SDK/STL/STL.h"

// Fast IO https://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
void StringFromFile(const string &path, string &contents)
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
string StringFromFile(const string &path)
{
	string contents;
	StringFromFile(path, contents);
	return contents;
}
template<typename T>
void VectorFromFile(const string &path, int beg, int count, vector<T> &contents)
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
vector<T> VectorFromFile(const string &path, int beg, int count)
{
	vector<T> contents;
	VectorFromFile<T>(path, beg, count, contents);
	return contents;
}

#endif