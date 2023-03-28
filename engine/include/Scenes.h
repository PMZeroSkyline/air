#ifndef SCENES_H
#define SCENES_H

#include "Core.h"
#include "GL.h"
#include "AC.h"
#include "Transform.h"
#include "GLTF.h"
#include "Blob.h"

class SceneNode
{
public:
    int mesh = -1;
    int skin = -1;
    int camera = -1;
    string name;
    Transform localTransform;
    vector<int> children;
};
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
    shared_ptr<GLProgram> glProgram;
    vector<pair<string, Texture2D*>> textures;
};
class MeshAttribute
{
public:
    vector<vec3> POSITION;
    vector<vec3> NORMAL;
    vector<vec4> TANGENT;
    vector<vec2> TEXCOORD_0;
    vector<vec2> TEXCOORD_1;
    vector<vec2> TEXCOORD_2;
    vector<vec2> TEXCOORD_3;
    vector<ivec4> JOINTS_0;
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
    string name;
    int skeleton;
    vector<mat4> inverseBindMatrices;
    vector<int> joints;
};
class Scene
{
public:
    string name;
    vector<int> nodes;
};
Blob<Image> imageBlob;
Blob<Texture2D> texture2DBlob;
Blob<GLProgram> glProgramBlob;
class Scenes
{
public:
    vector<SceneNode> nodes;

    vector<shared_ptr<Image>> images;
    vector<Sampler> samplers;
    vector<shared_ptr<Texture2D>> textures;
    vector<Material> materials;
    vector<Mesh> meshs;
    vector<Skin> skins;
    vector<Scene> scenes;
    int scene;
    string path;
    string dir;
    void Load(const string &_path)
    {
        gltf::glTF gltf = gltf::Load(_path);
        SetupPathAndDir(_path);
        SetupScenes(gltf);
        SetupSkins(gltf);
        SetupNodes(gltf);
        SetupMeshs(gltf);
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
    void SetupScenes(const gltf::glTF &gltf)
    {
        scene = gltf.scene;
        scenes.resize(gltf.scenes.size());
        for (int i = 0; i < gltf.scenes.size(); i++)
        {
            Scene &scene = scenes[i];
            const gltf::Scene &gScene = gltf.scenes[i];
            scene.name = gScene.name;
            scene.nodes = gScene.nodes;
        }
    }
    void SetupSkins(const gltf::glTF &gltf)
    {
        skins.resize(gltf.skins.size());
        for (int i = 0; i < gltf.skins.size(); i++)
        {
            Skin &skin = skins[i];
            const gltf::Skin &gSkin = gltf.skins[i];

            skin.name = move(gSkin.name);
            skin.skeleton = gSkin.skeleton;
            gltf::AccessResult result = gltf::Access(gltf, gSkin.inverseBindMatrices);
            if (result.accessor->componentType == GL_FLOAT && result.accessor->type == "MAT4")
            {
                VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, skin.inverseBindMatrices);VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, skin.inverseBindMatrices);
            }
            else
            {
                LOG("read inv bind mat failed, type not support !")
            }
            skin.joints = move(gSkin.joints);
        }
    }
    void SetupNodes(const gltf::glTF &gltf)
    {
        nodes.resize(gltf.nodes.size());
        for (int i = 0; i < gltf.nodes.size(); i++)
        {
            SceneNode &node = nodes[i];
            const gltf::Node &gNode = gltf.nodes[i];
            node.name = gNode.name;
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
                node.localTransform.translation = translation;
                node.localTransform.rotation = rotation;
                node.localTransform.scaling = scaling;
            }
            else
            {
                node.localTransform = Transform(matrix);
            }
            node.mesh = gNode.mesh;
            node.skin = gNode.skin;
            node.camera = gNode.camera;
            node.children = gNode.children;
        }
    }
    void SetupMaterials(const gltf::glTF &gltf)
    {
        materials.resize(gltf.materials.size());
        for (int i = 0; i < gltf.materials.size(); i++)
        {
            const gltf::Material& gMaterial = gltf.materials[i];
            Material &material = materials[i];

            shared_ptr<GLProgram> glProgram = glProgramBlob.Get("default");
            if (!glProgram)
            {
                glProgram = make_shared<GLProgram>();
                glProgramBlob.Set("default", glProgram);
            }
            material.glProgram = glProgram;
        }
    }
    void SetupMeshs(const gltf::glTF &gltf)
    {
        meshs.resize(gltf.meshes.size());
        for (int i = 0; i < gltf.meshes.size(); i++)
        {
            const gltf::Mesh &gMesh = gltf.meshes[i];
            Mesh &mesh = meshs[i];
            mesh.name = gMesh.name;
            mesh.primitives.resize(gMesh.primitives.size());
            for (int i = 0; i < gMesh.primitives.size(); i++)
            {
                SetupMeshPrimitive(gltf, &mesh.primitives[i], gMesh.primitives[i]);
            }
        }
    }
    void SetupMeshPrimitive(const gltf::glTF &gltf, MeshPrimitive *meshPrimitive, const gltf::MeshPrimitive &gMeshPrimitive)
    {
        SetupVertexAttributes(gltf, &meshPrimitive->attribute, gMeshPrimitive.attributes);
        SetupMeshIndices(gltf, gMeshPrimitive.indices, meshPrimitive->indices);
        meshPrimitive->material = &materials[gMeshPrimitive.material];
    }
    void SetupMeshIndices(const gltf::glTF &gltf, int meshPrimitiveIndicesId, vector<unsigned int> &indices)
    {
        gltf::AccessResult result = gltf::Access(gltf, meshPrimitiveIndicesId);
        if (result.accessor->componentType == GL_UNSIGNED_SHORT)
        {
            vector<unsigned short> u16v;
            VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, u16v);
            vector<unsigned int> u32v(begin(u16v), end(u16v));
            indices = move(u32v);
        }
        else
        {
            LOG("not support indices type " << result.accessor->componentType << " !")
        }
    }
    void SetupVertexAttributes(const gltf::glTF &gltf, MeshAttribute* meshAttribute, const vector<pair<string, int>> &attributes)
    {
        SetupVertexAttributeVec3(gltf, attributes, "POSITION", meshAttribute->POSITION);
        SetupVertexAttributeVec3(gltf, attributes, "NORMAL", meshAttribute->NORMAL);
        SetupVertexAttributeVec4(gltf, attributes, "TANGENT", meshAttribute->TANGENT);
        SetupVertexAttributeVec2(gltf, attributes, "TEXCOORD_0", meshAttribute->TEXCOORD_0);
        SetupVertexAttributeVec2(gltf, attributes, "TEXCOORD_1", meshAttribute->TEXCOORD_1);
        SetupVertexAttributeVec2(gltf, attributes, "TEXCOORD_2", meshAttribute->TEXCOORD_2);
        SetupVertexAttributeVec2(gltf, attributes, "TEXCOORD_3", meshAttribute->TEXCOORD_3);
        SetupVertexAttributeIvec4(gltf, attributes, "JOINTS_0", meshAttribute->JOINTS_0);
        SetupVertexAttributeVec4(gltf, attributes, "WEIGHTS_0", meshAttribute->WEIGHTS_0);

    }
    void SetupVertexAttributeVec2(const gltf::glTF &gltf, const vector<pair<string, int>> &attributes, const string &key, vector<vec2> &contents)
    {
        vector<pair<string, int>>::const_iterator it = find_if(attributes.begin(), attributes.end(), [key](pair<string, int> attribute){return attribute.first == key;});
        if (it != attributes.end())
        {
            gltf::AccessResult result = gltf::Access(gltf, (*it).second);
            if (result.accessor->componentType == GL_FLOAT && result.accessor->type == "VEC2")
            {
                VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, contents);
            }
            else
            {
                LOG("mesh attr not support !")
            }
        }
    }
    void SetupVertexAttributeVec3(const gltf::glTF &gltf, const vector<pair<string, int>> &attributes, const string &key, vector<vec3> &contents)
    {
        vector<pair<string, int>>::const_iterator it = find_if(attributes.begin(), attributes.end(), [key](pair<string, int> attribute){return attribute.first == key;});
        if (it != attributes.end())
        {
            gltf::AccessResult result = gltf::Access(gltf, (*it).second);
            if (result.accessor->componentType == GL_FLOAT && result.accessor->type == "VEC3")
            {
                VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, contents);
            }
            else
            {
                LOG("mesh attr not support !")
            }
        }
    }
    void SetupVertexAttributeVec4(const gltf::glTF &gltf, const vector<pair<string, int>> &attributes, const string &key, vector<vec4> &contents)
    {
        vector<pair<string, int>>::const_iterator it = find_if(attributes.begin(), attributes.end(), [key](pair<string, int> attribute){return attribute.first == key;});
        if (it != attributes.end())
        {
            gltf::AccessResult result = gltf::Access(gltf, (*it).second);
            if (result.accessor->componentType == GL_FLOAT && result.accessor->type == "VEC4")
            {
                VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, contents);
            }
            else
            {
                LOG("mesh attr not support !")
            }
        }
    }
    void SetupVertexAttributeIvec4(const gltf::glTF &gltf, const vector<pair<string, int>> &attributes, const string &key, vector<ivec4> &contents)
    {
        vector<pair<string, int>>::const_iterator it = find_if(attributes.begin(), attributes.end(), [key](pair<string, int> attribute){return attribute.first == key;});
        if (it != attributes.end())
        {
            gltf::AccessResult result = gltf::Access(gltf, (*it).second);
            if (result.accessor->componentType == GL_UNSIGNED_BYTE && result.accessor->type == "VEC4")
            {
                vector<u8vec4> u8v4;
                VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, u8v4);
                vector<ivec4> i32v4(begin(u8v4), end(u8v4));
                contents = move(i32v4);
            }
            else
            {
                LOG("mesh attr not support !")
            }
        }
    }
};
class MeshComponent : public Component
{
public:
    Mesh* mesh;
};
class SkinComponent : public Component
{
public:
    vector<Actor*>* actors;
    Skin* skin;
};
Blob<Scenes> scenesBlob;
class ScenesComponent : public Component
{
public:
    vector<Actor*> actors;
    shared_ptr<Scenes> scenes;
    void Load(const string &path)
    {
        shared_ptr<Scenes> sharedScenes = scenesBlob.Get(path);
        if (!sharedScenes)
        {
            sharedScenes = make_shared<Scenes>();
            sharedScenes->Load(path);
        }
        scenes = sharedScenes;
    }
    void FieldsExpand()
    {
        Scene &scene = scenes->scenes[scenes->scene];
        owner->name = scene.name;
        actors.resize(scenes->nodes.size());
        for (int i = 0; i < scene.nodes.size(); i++)
        {
            NodeExpand(owner, scene.nodes[i]);
        }
    }
    void NodeExpand(Actor* parent, int nodeId)
    {
        Actor* actor = parent->AddChild<Actor>();
        actors[nodeId] = actor;

        SceneNode &node = scenes->nodes[nodeId];
        actor->localTransform = node.localTransform;
        actor->name = node.name;

        if (node.mesh != -1)
        {
            MeshComponent* meshComponent = actor->AddComponent<MeshComponent>();
            meshComponent->mesh = &(scenes->meshs[node.mesh]);
        }
        if (node.skin != -1)
        {
            SkinComponent* skinComponent = actor->AddComponent<SkinComponent>();
            skinComponent->skin = &(scenes->skins[node.skin]);
            skinComponent->actors = &actors;
        }
        for (int i = 0; i < node.children.size(); i++)
        {
            NodeExpand(actor, node.children[i]);
        }
    }
};
#endif