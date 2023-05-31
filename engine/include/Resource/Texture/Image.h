#ifndef IMAGE
#define IMAGE

#include "SDK/STL/STL.h"
#include "Core/Log/Log.h"
#include "Platform/File/ImageFileRead.h"
#include "Core/Container/WeakMap.h"

class Image
{
public:
	string name;
	unsigned char *d;
	int w = 0, h = 0, n = 0;

	Image(){}
	Image(const string &path)
	{
		Load(path);
	}
	void Load(const string &path)
	{
		name = path;
		if (IsLoaded())
			stbi_image_free(d);
		d = stbi_load(path.c_str(), &w, &h, &n, 0);
		if (!d)
		{
			LOG("failed to load Image " << path << " !");
			return;
		}
	}
	bool IsLoaded()
	{
		return !(w == 0 && h == 0 && n == 0);
	}
	~Image()
	{
		if (!IsLoaded())
		{
			stbi_image_free(d);
		}
	}
};

WeakMap<Image> imageWeakMap;

#endif