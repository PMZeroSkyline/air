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
    vector<unique_ptr<Actor>> nodes;
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
    vector<Actor*> nodes;
    vector<Skin> skins;
    vector<Texture2D> textures;
    vector<Sampler> samplers;
    vector<Material> materials;
    vector<Image> images;
    vector<Scene> scenes;
    int scene;

    string path;
    string dir;

    void Load(const string &_path)
    {
        SetupPathAndDir(_path);
        
        gltf::glTF gltf = gltf::Load(path);

        meshs.resize(gltf.meshes.size());
        nodes.resize(gltf.nodes.size());
        skins.resize(gltf.skins.size());
        textures.resize(gltf.textures.size());
        samplers.resize(gltf.samplers.size());
        materials.resize(gltf.materials.size());
        images.resize(gltf.images.size());
        scenes.resize(gltf.scenes.size());

        scene = gltf.scene;

        for (int i = 0; i < gltf.scenes.size(); i++)
        {
            SetupScene(gltf, &scenes[i], i);
        }
    }
    void SetupPathAndDir(const string &_path)
    {
        path = _path;
        size_t pos = path.find_last_of('/');
        if (pos == string::npos)
        {
            dir = "";
        }
        else
        {
            dir = path.substr(0, pos + 1);
        }
    }
    void SetupScene(const gltf::glTF &gltf, Scene *scene, int sceneId)
    {
        const gltf::Scene &gScene = gltf.scenes[sceneId];

        scene->name = gScene.name;
        scene->nodes.resize(gScene.nodes.size());

        for (int i = 0; i < gScene.nodes.size(); i++)
        {
            scenes[sceneId].nodes[i] = make_unique<Actor>();
            SetupNode(gltf, scene->nodes[i].get(), gScene.nodes[i]);
        }
        for (int i = 0; i < gScene.nodes.size(); i++)
        {
            SetupSkin(gltf, gScene.nodes[i]);
        }
    }
    void SetupSkin(const gltf::glTF &gltf, int nodeId)
    {
        const gltf::Node &gNode = gltf.nodes[nodeId];
        if (gNode.skin != -1)
        {
            SkinComponent* skinComponent = nodes[nodeId]->AddComponent<SkinComponent>();
            skinComponent->skin = &skins[gNode.skin];
            SetupSkin(gltf, &skins[gNode.skin], gNode.skin);
        }
        for (int i = 0; i < gNode.children.size(); i++)
        {
            SetupSkin(gltf, gNode.children[i]);
        }
    }
    void SetupSkin(const gltf::glTF &gltf, Skin *skin, int skinId)
    {
        const gltf::Skin &gSkin = gltf.skins[skinId];
    }
    void SetupNode(const gltf::glTF &gltf, Actor* node, int nodeId)
    {
        const gltf::Node &gNode = gltf.nodes[nodeId];
        nodes[nodeId] = node;

        node->name = gNode.name;
        mat4 matrix;
	    quat rotation;
	    vec3 scaling;
	    vec3 translation;
        memcpy(&matrix, &(gNode.matrix), sizeof(float)*16);
        memcpy(&translation, &(gNode.translation), sizeof(float)*3);
        memcpy(&scaling, &(gNode.scale), sizeof(float)*3);
        memcpy(&rotation, &(gNode.rotation), sizeof(float)*4);
        if (matrix == mat4(1.f))
        {
            node->localTransform.translation = translation;
            node->localTransform.rotation = rotation;
            node->localTransform.scaling = scaling;
        }
        else
        {
            node->localTransform = Transform(matrix);
        }
        if (gNode.mesh != -1)
        {
            MeshComponent* meshComponent = node->AddComponent<MeshComponent>();
            meshComponent->mesh = &meshs[gNode.mesh];
            SetupMesh(gltf, &meshs[gNode.mesh], gNode.mesh);
        }
        for (int i = 0; i < gNode.children.size(); i++)
        {
            Actor* child = node->AddChild<Actor>();
            SetupNode(gltf, child, gNode.children[i]);
        }
    }
    void SetupMesh(const gltf::glTF &gltf, Mesh *mesh, int meshId)
    {
        const gltf::Mesh &gMesh = gltf.meshes[meshId];
        mesh->name = gMesh.name;
        mesh->primitives.resize(gMesh.primitives.size());
        for (int i = 0; i < gMesh.primitives.size(); i++)
        {
            SetupMeshPrimitive(gltf, &mesh->primitives[i], gMesh.primitives[i]);
        }
    }
    void SetupMeshPrimitive(const gltf::glTF &gltf, MeshPrimitive *meshPrimitive, const gltf::MeshPrimitive &gMeshPrimitive)
    {
        SetupMeshAttribute(gltf, &meshPrimitive->attribute, gMeshPrimitive.attributes);

        gltf::AccessResult result = gltf::Access(gltf, gMeshPrimitive.indices);
        
        
    }
    void SetupMeshAttribute(const gltf::glTF &gltf, MeshAttribute *meshAttribute, const vector<pair<string, int>> &attributes)
    {

    }
};

#endif