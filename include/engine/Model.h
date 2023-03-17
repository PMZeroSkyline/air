#ifndef MODEL_H
#define MODEL_H

#include "Core.h"
#include "GL.h"
#include "Image.h"
#include "AC.h"


class Mesh
{
public:	
};
namespace g
{
	map<string, weak_ptr<Mesh>> meshMap;
}
shared_ptr<Mesh> GetMesh(const string &path, int mesh_i, const json &GLTF)
{
	auto it = g::meshMap.find(path+":"+to_string(mesh_i));
	if (it == g::meshMap.end() || (it->second.use_count() == 0))
	{
		shared_ptr<Mesh> sp = make_shared<Mesh>();
		return sp;
	}
	else
	{
		return it->second.lock();
	}
}
class MeshComponent : public Component
{
	shared_ptr<Mesh> mesh;
};
class Skin
{

};


#endif