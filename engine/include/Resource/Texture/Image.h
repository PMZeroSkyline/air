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
		if (stbi_write_jpg(path.c_str(), width, hight, channel, data, 0) == 0)
		{
			LOG("failed to save Image " << path)
		}
	}
	void UpSizeToPowerOfTwo()
	{
		int log2w = log2(width);
		int log2h = log2(hight);
		int pow2w = pow(2, log2w + 1);
		int pow2h = pow(2, log2h + 1);
		int w = width == pow(2, log2w) ? width : pow2w;
		int h = hight == pow(2, log2h) ? hight : pow2h;
		Resize(w, h);
	}
	void DownSizeToPowerOfTwo()
	{
		int log2w = log2(width);
		int log2h = log2(hight);
		int pow2w = pow(2, log2w);
		int pow2h = pow(2, log2h);
		Resize(pow2w, pow2h);
	}
	void FitSizeToPowerOfTwo()
	{
		int log2w = log2(width);
		int log2h = log2(hight);
		int dw = pow(2, log2w);
		int dh = pow(2, log2h);
		int uw = pow(2, log2w + 1);
		int uh = pow(2, log2h + 1);
		int dwDelta = width - dw;
		int uwDelta = uw - width;
		int dhDelta = hight - dh;
		int uhDelta = uh - hight;
		int w = (dwDelta < uwDelta) ? dw : uw;
		int h = (dhDelta < uhDelta) ? dh : uh;
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


void ExecImagePowerOfTwo(const string& path)
{
	Image img;
	img.Load(path);
 	img.UpSizeToPowerOfTwo();
	img.Save(path);
}
#define EXEC_PO2	if (argc != 2)					\
					{								\
						LOG("argv : [path]")		\
						return 0;					\
					}								\
					ExecImagePowerOfTwo(argv[1]);	\
					LOG("Done")						\
					return 0;
					
#endif