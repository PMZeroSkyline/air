#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::list;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::ostream;
using std::to_string;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
using namespace glm;
ostream &operator<<(ostream &os, const vec3 v)
{
	for (size_t i = 0; i < 3; i++)
		os << v[i] << ",";
	return os;
}
ostream &operator<<(ostream &os, const vec4 v)
{
	for (size_t i = 0; i < 4; i++)
		os << (v[i] < 0.00001 ? 0 : v[i]) << ",";
	return os;
}
ostream &operator<<(ostream &os, const quat v)
{
	for (size_t i = 0; i < 4; i++)
		os << (v[i] < 0.00001 ? 0 : v[i]) << ",";
	return os;
}
ostream &operator<<(ostream &os, const mat4 &m)
{
	mat4 t = transpose(m);
	for (int i = 0; i < 4; i++)
		os << t[i] << endl;
	return os;
}

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "json/json.hpp"
using namespace nlohmann;

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define LOG(arg) cout << arg << endl;

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif
void CDAppleResourcesDir()
{
	#ifdef __APPLE__    
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	char path[PATH_MAX];
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
	{
		LOG("CD Apple Resources Dir")
	}
	CFRelease(resourcesURL);
	chdir(path);
	#endif
}

// 以下IO函数使用了快速读取方法 https://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
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
void VectorSizeof(int &size, const T &t)
{
	size += sizeof(t[0]) * t.size();
}
template<typename T, typename... Args>
void VectorSizeof(int &size, const T &t, const Args&... args)
{
	VectorSizeof(size, t);
	VectorSizeof(size, args...);
}

#endif