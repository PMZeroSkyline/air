#ifndef FILE_DIRECTORY_H
#define FILE_DIRECTORY_H

#include "Core/Log/Log.h"
#include "SDK/STL/STL.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

// void CDAppleResourcesDir()
// {
// 	#ifdef __APPLE__    
// 	CFBundleRef mainBundle = CFBundleGetMainBundle();
// 	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
// 	char path[PATH_MAX];
// 	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
// 	{
// 		LOG("CD Apple Resources Dir Failed")
// 	}
// 	CFRelease(resourcesURL);
// 	chdir(path);
// 	#endif
// }
void CDWorkspacePath(const string& workspaceFolderName, const string& relativePath = "")
{
	fs::path currentPath = fs::current_path();
	string currentPathStr = currentPath.string();
	size_t pos = currentPathStr.find(workspaceFolderName);
	if (pos != string::npos)
	{
		string rootDir = currentPathStr.substr(0, pos);
		fs::current_path(rootDir + workspaceFolderName + "/" + relativePath);
	}
	else
	{
		LOG("failed to cd dir " << workspaceFolderName << " , " << relativePath )
	}
}
void GetFolderFiles(const string& path, vector<string> &result, bool isRecursive = true)
{
    for (const auto& entry : fs::directory_iterator(path)) 
	{
        if (fs::is_regular_file(entry)) 
		{
			result.push_back(entry.path().string());
        } 
		if (isRecursive)
		{
			if (fs::is_directory(entry)) 
			{
        	    GetFolderFiles(entry.path().string(), result, isRecursive);
        	}
		}
    }
}
vector<string> GetFolderFiles(const string& path, bool isRecursive = true)
{
	vector<string> output;
	GetFolderFiles(path, output, isRecursive);
	return output;
}
#endif