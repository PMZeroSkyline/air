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
void CDResourcesDir()
{
	#ifdef __APPLE__    
		CDAppleResourcesDir();
		fs::current_path("../game/res");
	#endif
	#ifdef _WIN32
		fs::current_path("../res");
	#endif
}


#endif