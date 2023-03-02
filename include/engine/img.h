#ifndef IMG_H
#define IMG_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <string>
#include <iostream>
using std::string;

struct image
{
	unsigned char *d;
	int w = 0, h = 0, n = 0;
	
	image(const string path)
	{
		d = stbi_load(path.c_str(), &w, &h, &n, 0);
		if (!d)
			std::cout << "failed to load image " + path << std::endl;
	}
	~image()
	{
		stbi_image_free(d);
	}
};

#endif