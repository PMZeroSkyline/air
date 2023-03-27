#ifndef SCENES_H
#define SCENES_H

#include "Core.h"
#include "GL.h"
#include "AC.h"
#include "Transform.h"
#include "GLTF.h"



class Sampler
{
public:
    int magFilter = -1;
    int minFilter = -1;
    int wrapS = -1;
    int wrapT = -1;
};
class Texture2D
{
public:
    Sampler* sampler;
    Image* image;
    GLTexture2D glTexture2D;
};
class Material
{
public:
    GLProgram glProgram;
    vector<pair<string, Texture2D*>> textures;
};
class MeshAttribute
{
    vector<vec3> POSITION;
    vector<vec3> NORMAL;
    vector<vec4> TANGENT;
    vector<vec2> TEXCOORD_0;
    vector<vec2> TEXCOORD_1;
    vector<vec2> TEXCOORD_2;
    vector<vec2> TEXCOORD_3;
    vector<u8vec4> JOINTS_0;
    vector<vec4> WEIGHTS_0;
};
class MeshPrimitive
{
public:
    GLPrimitive glPrimitive;
    MeshAttribute attribute;
    vector<unsigned int> indices;
    Material* material;
};
class Mesh
{
public:
    string name;
    vector<MeshPrimitive> primitives;
};

class Skin
{
public:
    mat4 inverseBindMatrices;
    vector<Actor*> joints;
};

class Scene
{
public:
    string name;
    vector<Actor*> nodes;
};

class MeshComponent : public Component
{
public:
    Mesh* mesh;
};
class SkinComponent : public Component
{
public:
    Skin* skin;
};
class Scenes
{
public:
    vector<Mesh> meshs;
    vector<Actor> nodes;
    vector<Skin> skins;
    vector<Texture2D> textures;
    vector<Sampler> samplers;
    vector<Material> materials;
    vector<Image> images;

    int scene;
    vector<Scene> scenes;

    void Load(const string &path)
    {
        gltf::glTF gltf = gltf::Load(path);
        
    }
};

#endif