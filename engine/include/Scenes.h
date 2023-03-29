#ifndef SCENES_H
#define SCENES_H

#include "Core.h"
#include "GL.h"
#include "AC.h"
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
class Shader
{
public:
    GLProgram glProgram;
    void Load(const string &vsPath, const string &fsPath)
    {
        GLShader vsShader(GL_VERTEX_SHADER), fsShader(GL_FRAGMENT_SHADER);
        string vsStr, fsStr;
        StringFromFile(vsPath, vsStr);
        StringFromFile(fsPath, fsStr);
        vsShader.Compile(vsStr.c_str());
        fsShader.Compile(fsStr.c_str());
        glProgram.Link(vsShader.id, fsShader.id);
    }
    void Use()
    {
        glUseProgram(glProgram.id);
    }
    void SetBool(const string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(glProgram.id name.c_str()), (int)value); 
    }
    void SetInt(const string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(glProgram.id name.c_str()), value); 
    }
    void SetFloat(const string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(glProgram.id name.c_str()), value); 
    }
    void SetVec2(const string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(glProgram.id name.c_str()), 1, &value[0]); 
    }
    void SetVec2(const string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(glProgram.id name.c_str()), x, y); 
    }
    void SetVec3(const string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(glProgram.id name.c_str()), 1, &value[0]); 
    }
    void SetVec3(const string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(glProgram.id name.c_str()), x, y, z); 
    }
    void SetVec4(const string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(glProgram.id name.c_str()), 1, &value[0]); 
    }
    void SetVec4(const string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(glProgram.id name.c_str()), x, y, z, w); 
    }
    void SetMat2(const string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(glProgram.id name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void SetMat3(const string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(glProgram.id name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void SetMat4(const string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(glProgram.id name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};
Blob<Shader> shaderBlob;
class Material
{
public:
    int layer;
    shared_ptr<Shader> shader;
    vector<pair<string, Texture2D*>> textures;
    void UseDefaultShader()
    {
        shared_ptr<Shader> defaultShader = shaderBlob.Get("default");
        if (!defaultShader)
        {
            defaultShader = make_shared<Shader>();
            defaultShader->Load("shader/default_vs.glsl", "shader/default_fs.glsl");
            shaderBlob.Set("default", defaultShader);
        }
        shader = defaultShader;
    }
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
class BoundingBox
{
public:
    vec3 min = vec3(0.f);
    vec3 max = vec3(0.f);
};
class MeshPrimitive
{
public:
    GLPrimitive glPrimitive;
    MeshAttribute attribute;
    vector<unsigned int> indices;
    int indicesSize;
    Material* material;
    BoundingBox boundingBox;
    void SetupGLPrimitive()
    {
        glPrimitive.Bind();
        GLVaoData(attribute.POSITION, attribute.NORMAL, attribute.TANGENT, attribute.TEXCOORD_0, attribute.TEXCOORD_1, attribute.TEXCOORD_2, attribute.TEXCOORD_3, attribute.JOINTS_0, attribute.WEIGHTS_0);
        GLEboData(indices);
    }
    void Draw()
    {
        glPrimitive.Bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
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
class AnimationSampler
{
public:
    vector<float> input;
	string interpolation;
    float min;
    float max;
	vector<vec3> outputVec3;
	vector<quat> outputQuat;
    bool GetInterpStep(float time, int &beg, int &end, float &interp)
    {
        vector<float>::iterator it = find_if(input.begin(), input.end(), [time](float inputValue){return inputValue >= time;});
        if (it != input.end())
        {
            end = it - input.begin();
            if (end = 0)
            {
                beg = 0;
                interp = 0;
                return true;
            }
            else if (end > 0)
            {
                float prog = time - input[beg];
                float range = input[end] - input[beg];
                interp = prog / range;
                beg = end - 1;
            }
            else
            {
                LOG("anim sampler sample input endId < 0 ? ");
                return false;
            }
        }
        else
        {
            LOG("anim sampler sample out of range");
            return false;
        }
    }
    vec3 SampleVec3(float time)
	{
        int beg, end;
        float interp;
        if (GetInterpStep(time, beg, end, interp))
        {
            vec3 a = outputVec3[beg];
            vec3 b = outputVec3[end];
            return mix(a, b, interp);
        }
        else
        {
            return vec3();
        }
	}
	quat SampleQuat(float time)
	{
		int beg, end;
        float interp;
        if (GetInterpStep(time, beg, end, interp))
        {
            quat a = outputQuat[beg];
            quat b = outputQuat[end];
            return slerp(a, b, interp);
        }
        else
        {
            return quat();
        }
	}
};
class AnimationChannelTarget
{
public:
    int node = -1;
    string path;
};
class AnimationChannel
{
public:
    AnimationSampler* sampler;
    AnimationChannelTarget target;
};
class Animation
{
public:
    string name;
    float min;
    float max;
    vector<AnimationChannel> channels;
    vector<AnimationSampler> samplers;
};
class Scene
{
public:
    string name;
    vector<int> nodes;
};
Blob<Image> imageBlob;
Blob<Texture2D> texture2DBlob;
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
    vector<Animation> animations;
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
        SetupAnimations(gltf);
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
            Material* material = &materials[i];
            material->UseDefaultShader();
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
        SetupMeshIndices(gltf, gMeshPrimitive.indices, meshPrimitive);
        meshPrimitive->material = &materials[gMeshPrimitive.material];
        SetupMeshPrimitiveBoundingBox(gltf, meshPrimitive, gMeshPrimitive);
        meshPrimitive->SetupGLPrimitive();
    }
    void SetupMeshPrimitiveBoundingBox(const gltf::glTF &gltf, MeshPrimitive *meshPrimitive, const gltf::MeshPrimitive &gMeshPrimitive)
    {
        vector<pair<string, int>>::const_iterator it = find_if(gMeshPrimitive.attributes.begin(), gMeshPrimitive.attributes.end(), [](pair<string, int> attribute){return attribute.first == "POSITION";});
        if (it != gMeshPrimitive.attributes.end())
        {
            gltf::AccessResult result = gltf::Access(gltf, (*it).second);
            if (result.accessor->min.size() == 3 && result.accessor->max.size() == 3)
            {
                memcpy(&(meshPrimitive->boundingBox.min), &(result.accessor->min[0]), sizeof(float)*3);
                memcpy(&(meshPrimitive->boundingBox.max), &(result.accessor->max[0]), sizeof(float)*3);
            }
        }
    }
    void SetupMeshIndices(const gltf::glTF &gltf, int meshPrimitiveIndicesId, MeshPrimitive *meshPrimitive)
    {
        gltf::AccessResult result = gltf::Access(gltf, meshPrimitiveIndicesId);
        if (result.accessor->componentType == GL_UNSIGNED_SHORT)
        {
            vector<unsigned short> u16v;
            VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, u16v);
            vector<unsigned int> u32v(begin(u16v), end(u16v));
            meshPrimitive->indices = move(u32v);
            meshPrimitive->indicesSize = meshPrimitive->indices.size();
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
        vector<pair<string, int>>::const_iterator it = find_if(attributes.begin(), attributes.end(), [&key](pair<string, int> attribute){return attribute.first == key;});
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
        vector<pair<string, int>>::const_iterator it = find_if(attributes.begin(), attributes.end(), [&key](pair<string, int> attribute){return attribute.first == key;});
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
        vector<pair<string, int>>::const_iterator it = find_if(attributes.begin(), attributes.end(), [&key](pair<string, int> attribute){return attribute.first == key;});
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
        vector<pair<string, int>>::const_iterator it = find_if(attributes.begin(), attributes.end(), [&key](pair<string, int> attribute){return attribute.first == key;});
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
    void SetupAnimations(const gltf::glTF &gltf)
    {
        animations.resize(gltf.animations.size());
        for (int i = 0; i < gltf.animations.size(); i++)
        {
            Animation &animation = animations[i];
            const gltf::Animation &gAnimation = gltf.animations[i];

            animation.name = gAnimation.name;
            SetupAnimationSamplers(gltf, &animation, gAnimation);
            SetupAnimationChannels(gltf, &animation, gAnimation);
        }
    }
    void SetupAnimationSamplers(const gltf::glTF &gltf, Animation* animation, const gltf::Animation &gAnimation)
    {
        animation->samplers.resize(gAnimation.samplers.size());
        for (int i = 0; i < gAnimation.samplers.size(); i++)
        {
            AnimationSampler &sampler = animation->samplers[i];
            const gltf::AnimationSampler &gSampler = gAnimation.samplers[i];
            
            sampler.interpolation = gSampler.interpolation;
            
            gltf::AccessResult result = gltf::Access(gltf, gSampler.input);
            VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, sampler.input);
            if (result.accessor->min.size() == 1 && result.accessor->max.size() == 1)
            {
                sampler.min = result.accessor->min[0];
                sampler.max = result.accessor->max[0];
            }
            else
            {
                LOG("not find min max in animation sampler access result !")
            }
            if (i == 0)
            {
                animation->min = sampler.min;
                animation->max = sampler.max;
            }
            else
            {
                animation->min = min(animation->min, sampler.min);
                animation->max = max(animation->max, sampler.max);
            }
            result = gltf::Access(gltf, gSampler.output);
            if (result.accessor->type == "VEC3")
            {
                VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, sampler.outputVec3);
            }
            else if (result.accessor->type == "VEC4")
            {
                VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, sampler.outputQuat);
            }
            else
            {
                LOG("not find animation sampler output type !")
            }
        }
    }
    void SetupAnimationChannels(const gltf::glTF &gltf, Animation* animation, const gltf::Animation &gAnimation)
    {
            animation->channels.resize(gAnimation.channels.size());
            for (int i = 0; i < gAnimation.channels.size(); i++)
            {
                animation->channels[i].sampler = &(animation->samplers[gAnimation.channels[i].sampler]);
                animation->channels[i].target.node = gAnimation.channels[i].target.node;
                animation->channels[i].target.path = gAnimation.channels[i].target.path;
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
class AnimationChannelsComponent : public Component
{
public:
    vector<pair<float*, AnimationChannel*>> channelPairs;
};
Blob<Scenes> scenesBlob;
class ScenesComponent : public Component
{
public:
    vector<Actor*> actors;
    shared_ptr<Scenes> scenes;
    vector<float> animationWeights;
    void Load(const string &path)
    {
        shared_ptr<Scenes> sharedScenes = scenesBlob.Get(path);
        if (!sharedScenes)
        {
            sharedScenes = make_shared<Scenes>();
            sharedScenes->Load(path);
            scenesBlob.Set(path, sharedScenes);
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
            SceneNodeExpand(owner, scene.nodes[i]);
        }
        animationWeights.resize(scenes->animations.size());
        for (int i = 0; i < scenes->animations.size(); i++)
        {
            Animation* animation = &(scenes->animations[i]);
            for (int j = 0; j < animation->channels.size(); j++)
            {
                AnimationChannel* channel = &(animation->channels[j]);
                Actor* markForAddChannelActor = actors[channel->target.node];
                AnimationChannelsComponent* channelsComponent = markForAddChannelActor->GetComponent<AnimationChannelsComponent>();
                if (!channelsComponent)
                {
                    channelsComponent = markForAddChannelActor->AddComponent<AnimationChannelsComponent>();
                }
                channelsComponent->channelPairs.push_back(make_pair(&animationWeights[i], channel));
            }
        }
    }
    void SceneNodeExpand(Actor* parent, int nodeId)
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
            SceneNodeExpand(actor, node.children[i]);
        }
    }
};

#endif