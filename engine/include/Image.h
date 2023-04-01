#ifndef IMAGE_H
#define IMAGE_H

#include "Core.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
			LOG("failed to load Image !");
			return;
		}
		LOG("load Image succeed");
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

#endif