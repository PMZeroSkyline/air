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
	unsigned char* data;
	int width = 0, hight = 0, channel = 0;

	Image(){}
	Image(const string &path)
	{
		Load(path);
	}
	void Load(const string& path)
	{
		name = path;
		if (IsLoaded())
			stbi_image_free(data);
		data = stbi_load(path.c_str(), &width, &hight, &channel, 0);
		if (!data)
		{
			LOG("failed to load Image " << path << " !")
			return;
		}
	}
	void Save(const string& path)
	{
		if (channel <= 3)
		{
			if (stbi_write_jpg(path.c_str(), width, hight, channel, data, 0) == 0)
			{
				LOG("failed to save Image " << path)
			}
		}
		if (channel == 4)
		{
			if (stbi_write_png(path.c_str(), width, hight, channel, data, 0) == 0)
			{
				LOG("failed to save Image " << path)
			}
		}
	}
	bool IsPowerOfTwo()
	{
		return (pow(2, log2(width)) == width && pow(2, log2(hight)) == hight);
	}
	bool GetPowerOfTwoBoundSize(int& w, int& h)
	{
		int lw = log2(width);
		int lh = log2(hight);
		w = width == pow(2, lw) ? width : pow(2, lw + 1);
		h = hight == pow(2, lh) ? hight : pow(2, lh + 1);
	}
	void GetPowerOfTwoFloorSize(int& w, int& h)
	{
		w = pow(2, log2(width));
		h = pow(2, log2(hight));
	}
	void Resize(int newWidth, int newHight)
	{
		if (!IsLoaded())
		{
			return;
		}
		unsigned char* newData = (unsigned char*)malloc(newWidth * newHight * channel);
		stbir_resize_uint8(data, width, hight, 0, newData, newWidth, newHight, 0, channel);
		stbi_image_free(data);
		data = newData;
		width = newWidth;
		hight = newHight;
	}
	void Reserv(int newWidth, int newHight)
	{
		if (!IsLoaded())
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
					bool isInside = u < width && v < hight;
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
		stbi_image_free(data);
		data = newData;
		width = newWidth;
		hight = newHight;
	}
	bool IsLoaded()
	{
		return (width != 0 && hight != 0 && channel != 0);
	}
	~Image()
	{
		if (!IsLoaded())
		{
			stbi_image_free(data);
		}
	}
};

WeakMap<Image> imageWeakMap;




					
#endif