#include <iostream>
#include "Window.h"
#include "Image.h"
#include "GL.h"
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

int main()
{
	CDAppleResourcesDir();
	fs::current_path("../res");
    std::cout << "Current working directory: " << fs::current_path() << '\n'; 
 	// read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("sushi_bar/scene.gltf", aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return 0;
    }
	vec3 e(0.f ,90.f, 0.f);
	quat q = EulerToQuat(radians(e));
	e = degrees(eulerAngles(q));
	cout << e << endl;
	// Result : 0,89.9802,0,

	Window window;
	
	while (window.IsOpen())
	{
		window.Tick();

		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
		
	}

	return 0;
}