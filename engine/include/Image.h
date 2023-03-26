#ifndef IMAGE_H
#define IMAGE_H

#include "Core.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Image
{
public:
	unsigned char *d;
	int w = 0, h = 0, n = 0;
	Image(const string path)
	{
		d = stbi_load(path.c_str(), &w, &h, &n, 0);
		if (!d)
		{
			LOG("failed to load Image !");
		}
		LOG("load Image succeed");
	}
	~Image()
	{
		stbi_image_free(d);
	}
};

#endif