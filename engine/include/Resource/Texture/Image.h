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
	bool UpSizeToPowerOfTwo(bool isCheckDiff = true)
	{
		int log2w = log2(width);
		int log2h = log2(hight);
		int w, h;
		if (isCheckDiff)
		{
			w = width == pow(2, log2w) ? width : pow(2, log2w + 1);
			h = hight == pow(2, log2h) ? hight : pow(2, log2h + 1);
			if (w == width && h == hight)
			{
				return false;
			}
		}
		else
		{
			w = pow(2, log2w + 1);
			h = pow(2, log2h + 1);
		}
		Resize(w, h);
		return true;
	}
	void FloorSizeToPowerOfTwo()
	{
		int log2w = log2(width);
		int log2h = log2(hight);
		int pow2w = pow(2, log2w);
		int pow2h = pow(2, log2h);
		Resize(pow2w, pow2h);
	}
	void DownSizeToPowerOfTwo()
	{
		int log2w = log2(width);
		int log2h = log2(hight);
		int pow2w = pow(2, log2w - 1);
		int pow2h = pow(2, log2h - 1);
		Resize(pow2w, pow2h);
	}
	void RoundSizeToPowerOfTwo()
	{
		int log2w = log2(width);
		int log2h = log2(hight);
		int wFloor = pow(2, log2w);
		int hFloor = pow(2, log2h);
		int wUp = pow(2, log2w + 1);
		int hUp = pow(2, log2h + 1);
		int w = ((width - wFloor) < (wUp - width)) ? wFloor : wUp;
		int h = ((hight - hFloor) < (hUp - hight)) ? hFloor : hUp;
		Resize(w, h);
	}
	void Resize(int newWidth, int newHight)
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