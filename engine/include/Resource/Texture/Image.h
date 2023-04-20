#ifndef IMAGE
#define IMAGE

#include "SDK/STL/STL.h"
#include "Core/Log/Log.h"
#include "Platform/File/ImageFileRead.h"
#include "Resource/Container/Blob.h"

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

Blob<Image> imageBlob;

#endif