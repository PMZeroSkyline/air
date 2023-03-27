#ifndef IMAGE_H
#define IMAGE_H

#include "Core.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Image
{
public:
	string path;
	unsigned char *d;
	int w = 0, h = 0, n = 0;

	Image(){}
	Image(const string &path)
	{
		Load(path);
	}
	void Load(const string &_path)
	{
		if (path != "")
			stbi_image_free(d);
		d = stbi_load(_path.c_str(), &w, &h, &n, 0);
		if (!d)
		{
			LOG("failed to load Image !");
			return;
		}
		path = _path;
		LOG("load Image succeed");
	}
	~Image()
	{
		if (path == "")
			return;
		stbi_image_free(d);
	}
};

#endif