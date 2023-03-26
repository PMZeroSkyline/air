#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <tuple>
#include <limits>
#include <filesystem>
using std::cout;
using std::endl;
using std::begin;
using std::end;
using std::string;
using std::ifstream;
using std::vector;
using std::tuple;
using std::map;
using std::list;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::weak_ptr;
using std::ostream;
using std::to_string;
using std::move;
using std::dynamic_pointer_cast;
using std::numeric_limits;
using std::pair;
using std::make_pair;
namespace fs = std::filesystem;

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
	for (int i = 0; i < 4; i++)
		os << m[i] << endl;
	return os;
}
quat EulerToQuat(float roll, float pitch, float yaw) // roll (x), pitch (Y), yaw (z)
{
	// https://en.wikipedia.org/wiki/Conversion_between_quats_and_Euler_angles

    quat q;

    float cr = cos(roll * 0.5);
    float sr = sin(roll * 0.5);
    float cp = cos(pitch * 0.5);
    float sp = sin(pitch * 0.5);
    float cy = cos(yaw * 0.5);
    float sy = sin(yaw * 0.5);

    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;
    return q;
}
quat EulerToQuat(const vec3 &v)
{
    return EulerToQuat(v.x, v.y, v.z);
}

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "single_include/nlohmann/json.hpp"
using namespace nlohmann;


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
		LOG("CD Apple Resources Dir Failed")
	}
	CFRelease(resourcesURL);
	chdir(path);
	#endif
}
void CDResourcesDir()
{
	#ifdef __APPLE__    
		CDAppleResourcesDir();
		fs::current_path("../game/res");
	#endif
	#ifdef _WIN32
		fs::current_path("../res");
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
void SplitToVector(const string &target, const string &splitTag, vector<string> &out)
{
    size_t offset = 0;
    size_t found;
    while ((found = target.find(splitTag, offset)) != string::npos)
    {
        out.push_back(target.substr(offset, found-offset));
        offset = found+1;
    }
    out.push_back(target.substr(offset, target.size()-offset));
}
void RemoveSpace(string &str)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}
void ReadCSV(const string &path, vector<vector<string>> &csv, bool removeSpace = false)
{
    ifstream csvFile(path);
    string line;
    while (getline(csvFile, line))
    {
        if (removeSpace)
            RemoveSpace(line);
        vector<string> v;
        csv.push_back(v);
        SplitToVector(line, ",", csv.back());
    }
}
#endif