#ifndef FILE_DIRECTORY_H
#define FILE_DIRECTORY_H

#include "Core/Log/Log.h"
#include "SDK/STL/STL.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

void CDAppleResourcesDir()
{
	#ifdef __APPLE__    
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	char path[PATH_MAX];
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
	{
		LOG("CD Apple Resources Dir Failed")
	}
	CFRelease(resourcesURL);
	chdir(path);
	#endif
}
void CDResDir()
{
	fs::path currentPath = fs::current_path();
	string currentPathStr = currentPath.string();
	size_t pos = currentPathStr.find("air");
	if (pos != string::npos)
	{
		string rootDir = currentPathStr.substr(0, pos);
		//fs::current_path(rootDir + "air/game/res");
		fs::current_path(rootDir + "air/game/res");
	}
	else
	{
		LOG("failed to cd resources dir")
	}

	// #ifdef __APPLE__    
	// 	CDAppleResourcesDir();
	// 	fs::current_path("../game/res");
	// #endif
	// #ifdef _WIN32
		
	// #endif
}

#endif