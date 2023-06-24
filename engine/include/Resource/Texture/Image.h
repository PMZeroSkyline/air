#ifndef IMAGE_H
#define IMAGE_H 

#include "SDK/STL/STL.h"
#include "Core/Log/Log.h"
#include "Platform/File/ImageFile.h"
#include "Core/Container/WeakMap.h"

class Image
{
public:
	string name;
	unsigned char* data = nullptr;
	float* hdrData = nullptr;
	int width = 0, height = 0, channel = 0;

	Image(){}
	Image(const string &path)
	{
		Load(path);
	}
	void Free()
	{
		if (data)
		{
			stbi_image_free(data);
			data = nullptr;
		}
		if (hdrData)
		{
			stbi_image_free(hdrData);
			hdrData = nullptr;
		}
	}
	bool IsPowerOfTwo()
	{
		return (pow(2, log2(width)) == width && pow(2, log2(height)) == height);
	}
	void GetCeilingPowerOfTwoSize(int& w, int& h)
	{
		int lw = log2(width);
		int lh = log2(height);
		w = width == pow(2, lw) ? width : pow(2, lw + 1);
		h = height == pow(2, lh) ? height : pow(2, lh + 1);
	}
	void GetFloorPowerOfTwoSize(int& w, int& h)
	{
		int lw = log2(width);
		int lh = log2(height);
		w = pow(2, lw);
		h = pow(2, lh);
	}
	void Load(const string& path)
	{
		Free();

		name = path;
		fs::path filePath = path;
		string ext = filePath.extension().string();
		if (ext == ".png" || ext == ".jpg")
		{
			data = stbi_load(path.c_str(), &width, &height, &channel, 0);
			if (!data)
			{
				LOG("failed to load Image " << path << " !")
				Free();
				return;
			}
			if (!IsPowerOfTwo())
			{
				int x, y;
				GetFloorPowerOfTwoSize(x, y);
				Resize(x, y);
			}
		}
		else if (ext == ".hdr")
		{
			hdrData = stbi_loadf(path.c_str(), &width, &height, &channel, 0);
			if (!hdrData)
			{
				LOG("failed to load hdr Image " << path << " !")
				Free();
				return;
			}
		}
		else
		{
			LOG("failed to load Image " << path << " !")
		}
	}
	void Save(const string& path)
	{
		if (!data)
		{
			return;
		}
		if (channel <= 3)
		{
			if (stbi_write_jpg(path.c_str(), width, height, channel, data, 0) == 0)
			{
				LOG("failed to save Image " << path)
			}
		}
		if (channel == 4)
		{
			if (stbi_write_png(path.c_str(), width, height, channel, data, 0) == 0)
			{
				LOG("failed to save Image " << path)
			}
		}
	}
	void Resize(int newWidth, int newHight)
	{
		if (!data)
		{
			return;
		}
		unsigned char* newData = (unsigned char*)malloc(newWidth * newHight * channel);
		stbir_resize_uint8(data, width, height, 0, newData, newWidth, newHight, 0, channel);
		stbi_image_free(data);
		data = newData;
		width = newWidth;
		height = newHight;
	}
	void Reserv(int newWidth, int newHight)
	{
		if (!data)
		{
			return;
		}
		unsigned char* newData = (unsigned char*)malloc(newWidth * newHight * channel);
		for (int v = 0; v < newHight; v++)
		{
			for (int u = 0; u < newWidth; u++)
			{
				for (int c = 0; c < channel; c++)
				{
					bool isInside = u < width && v < height;
					if (isInside)
					{
						*(newData + (v * newWidth + u) * channel + c) = *(data + (v * width + u) * channel + c);
					}
					else
					{
						*(newData + (v * newWidth + u) * channel + c) = 0;
					}
				}
			}
		}
		Free();
		data = newData;
		width = newWidth;
		height = newHight;
	}
	~Image()
	{
		Free();
	}
};

WeakMap<Image> imageWeakMap;




					
#endif