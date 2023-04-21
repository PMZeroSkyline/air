#ifndef SCENES_H
#define SCENES_H

#include "Core/Log/Log.h"
#include "SDK/STL/STL.h"
#include "Core/Parse/JSONFileParse.h"
#include "Core/Parse/GLTFFileParse.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Resource/Model/Mesh.h"
#include "Resource/Material/Material.h"
#include "Resource/Texture/Texture2D.h"
#include "Resource/Skin/Skin.h"
#include "Resource/Skin/Animation.h"
#include "Resource/Container/Blob.h"

class Scenes
{
public:
    int sceneID;
    vector<Scene> scenes;
    vector<SceneNode> nodes;
    vector<shared_ptr<Mesh>> meshs;
    vector<shared_ptr<Material>> materials;
    vector<shared_ptr<Texture2D>> texture2Ds;
    vector<shared_ptr<Skin>> skins;
    vector<shared_ptr<Animation>> animations;
    string path;
    string dir;
    void Load(const string &_path)
    {
        path = _path;
        size_t found = path.find_last_of('/');
        dir = path.substr(0, found != string::npos ? found + 1 : 0);
        gltf::glTF GLTF = gltf::Load(path);

        sceneID = GLTF.scene;
         
        scenes.resize(GLTF.scenes.size());
        nodes.resize(GLTF.nodes.size());
        materials.resize(GLTF.materials.size());
        meshs.resize(GLTF.meshes.size());
        texture2Ds.resize(GLTF.textures.size());
        skins.resize(GLTF.skins.size());
        animations.resize(GLTF.animations.size());

        for (int i = 0; i < GLTF.animations.size(); i++)
        {
            SetupAnimation(GLTF, i);
        }
        for (int i = 0; i < GLTF.skins.size(); i++)
        {
            SetupSkin(GLTF, i);
        }
        for (int i = 0; i < GLTF.textures.size(); i++)
        {
            SetupTexture(GLTF, i);
        }
        for (int i = 0; i < GLTF.materials.size(); i++)
        {
            materials[i] = make_shared<Material>();
            SetupMaterial(GLTF, i);
        }
        for (int i = 0; i < GLTF.meshes.size(); i++)
        {
            SetupMesh(GLTF, i);
        }
        for (int i = 0; i < GLTF.nodes.size(); i++)
        {
            SetupNode(GLTF, i);
        }
        for (int i = 0; i < GLTF.scenes.size(); i++)
        {
            SetupScene(GLTF, i);
        }
    }
    void SetupAnimation(const gltf::glTF& GLTF, int i)
    {
        const gltf::Animation* gAnimation = &GLTF.animations[i];
        shared_ptr<Animation> animation = make_shared<Animation>();
        animations[i] = animation;

        animation->samplers.resize(gAnimation->samplers.size());
        for (int i = 0; i < gAnimation->samplers.size(); i++)
        {
            AnimationSampler* animationSampler = &animation->samplers[i];
            const gltf::AnimationSampler* gAnimationSampler = &gAnimation->samplers[i];
            gltf::AccessResult result = gltf::Access(GLTF, gAnimationSampler->input);
            if (result.accessor->componentType == GL_FLOAT)
            {
                VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, animationSampler->input);
            }
            if (result.accessor->min.size() == 1)
            {
                animationSampler->inputMin = result.accessor->min[0];
                if (animation->samplersInputMin == -1)
                {
                    animation->samplersInputMin = animationSampler->inputMin;
                }
                else
                {
                    animation->samplersInputMin = min(animation->samplersInputMin, animationSampler->inputMin);
                }
            }
            else
            {
                LOG("Load animation lost SamplersInputMin !")
            }
            if (result.accessor->max.size() == 1)
            {
                animationSampler->inputMax = result.accessor->max[0];
                if (animation->samplersInputMax == -1)
                {
                    animation->samplersInputMax = animationSampler->inputMax;
                }
                else
                {
                    animation->samplersInputMax = max(animation->samplersInputMax, animationSampler->inputMax);
                }
            }
            else
            {
                LOG("Load animation lost SamplersInputMax !")
            }
            result = gltf::Access(GLTF, gAnimationSampler->output);
            if (result.accessor->componentType == GL_FLOAT && result.accessor->type == "VEC3")
            {
                VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, animationSampler->outputVec3);
            }
            else if (result.accessor->componentType == GL_FLOAT && result.accessor->type == "VEC4")
            {
                VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, animationSampler->outputQuat);
            }
        }

        animation->name = gAnimation->name;
        animation->channels.resize(gAnimation->channels.size());
        for (int i = 0; i < gAnimation->channels.size(); i++)
        {
            AnimationChannel* animationChannel = &animation->channels[i];
            const gltf::AnimationChannel* gAnimationChannel = &gAnimation->channels[i];
            animationChannel->sampler = &animation->samplers[gAnimationChannel->sampler];
            animationChannel->target.nodeID = gAnimationChannel->target.node;
            animationChannel->target.SetupPath(gAnimationChannel->target.path);
        }
    }
    void SetupSkin(const gltf::glTF& GLTF, int i)
    {
        const gltf::Skin* gSkin = &GLTF.skins[i];
        shared_ptr<Skin> skin = make_shared<Skin>();
        skins[i] = skin;

        skin->name = gSkin->name;
        gltf::AccessResult result = gltf::Access(GLTF, gSkin->inverseBindMatrices);
        if (result.accessor->componentType == GL_FLOAT)
        {
            VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, skin->inverseBindMatrices);
        }
        for (int i = 0; i < skin->inverseBindMatrices.size(); i++)
        {
            skin->inverseBindMatrices[i] = skin->inverseBindMatrices[i].transpose();
        }
        skin->jointIDs.resize(gSkin->joints.size());
        memcpy(&skin->jointIDs[0], &gSkin->joints[0], sizeof(int)*gSkin->joints.size());
    }
    void SetupTexture(const gltf::glTF& GLTF, int i)
    {
        const gltf::Texture* gTexture = &GLTF.textures[i];
        const gltf::Sampler* gSampler = &GLTF.samplers[gTexture->sampler];
        const gltf::Image* gImage = &GLTF.images[gTexture->source];
        string key = dir + gImage->uri + ":" + gSampler->ToString();
        shared_ptr<Texture2D> texture2D = texture2DBlob.Get(key);
        if (texture2D)
        {
            return;
        }
        texture2D = make_shared<Texture2D>();
        texture2Ds[i] = texture2D;
        texture2DBlob.Set(key, texture2D);

        texture2D->name = gTexture->name;
        if (gTexture->sampler != -1)
        {
            const gltf::Sampler* gSampler = &GLTF.samplers[gTexture->sampler];
            if (gSampler->magFilter != -1)
            {
                texture2D->sampler.magFilter = gSampler->magFilter;
            }
            if (gSampler->minFilter != -1)
            {
                texture2D->sampler.minFilter = gSampler->minFilter;
            }
            texture2D->sampler.wrapS = gSampler->wrapS;
            texture2D->sampler.wrapT = gSampler->wrapT;
        }
        if (gTexture->source != -1)
        {
            const gltf::Image* gImage = &GLTF.images[gTexture->source];
            shared_ptr<Image> image = imageBlob.Get(dir + gImage->uri);
            if (!image)
            {
                image = make_shared<Image>();
                image->name = gImage->name;
                image->Load(dir + gImage->uri);
                imageBlob.Set(dir + gImage->uri, image);
            }
            texture2D->image = image;
        }
        else
        {
            LOG("filed to find gltf texture " << gTexture->name << " source !")
        }
        texture2D->SetupGLTexture2D();
    }
    void SetupMaterial(const gltf::glTF& GLTF, int i)
    {
        const gltf::Material* gMaterial = &GLTF.materials[i];
        shared_ptr<Material> material = make_shared<Material>();
        materials[i] = material;

        material->name = gMaterial->name;
        material->UseDefaultShader();
        material->alphaMode =   gMaterial->alphaMode == "OPAQUE" ? MaterialAlphaMode::OPAQUE : 
                                gMaterial->alphaMode == "MASK" ? MaterialAlphaMode::MASK : 
                                gMaterial->alphaMode == "BLEND" ? MaterialAlphaMode::BLEND : MaterialAlphaMode::OPAQUE;
        material->doubleSided = gMaterial->doubleSided;
        material->alphaCutoff = gMaterial->alphaCutoff;
        
        if (gMaterial->pbrMetallicRoughness.baseColorTexture.index != -1)
        {
            material->texturePairs.push_back(make_pair("baseColorTex", texture2Ds[gMaterial->pbrMetallicRoughness.baseColorTexture.index]));
        }
    }
    void SetupMesh(const gltf::glTF& GLTF, int i)
    {
        const gltf::Mesh* gMesh = &GLTF.meshes[i];
        shared_ptr<Mesh> mesh = make_shared<Mesh>();
        meshs[i] = mesh;
        
        mesh->name = gMesh->name;
        mesh->primitives.resize(gMesh->primitives.size());
        for (int i = 0; i < gMesh->primitives.size(); i++)
        {
            const gltf::MeshPrimitive* gPrimitive = &gMesh->primitives[i];
            MeshPrimitive* primitive = &mesh->primitives[i];
            
            // POSITION
            // NORMAL
            // TANGENT
            // TEXCOORD_0
            // TEXCOORD_1
            // TEXCOORD_2
            // TEXCOORD_3
            // JOINTS_0
            // WEIGHTS_0
            int id;
            id = gPrimitive->Find("POSITION");
            if (id != -1)
            {
                gltf::AccessResult result = gltf::Access(GLTF, id);
                if (result.accessor->componentType == GL_FLOAT)
                {
                    VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, primitive->attribute.POSITION);
                }
                if (result.accessor->min.size() == 3 && result.accessor->max.size() == 3)
                {
                    memcpy(&primitive->boundingBox.min, &result.accessor->min[0], sizeof(vec3));
                    memcpy(&primitive->boundingBox.max, &result.accessor->max[0], sizeof(vec3));
                }
            }
            id = gPrimitive->Find("TEXCOORD_0");
            if (id != -1)
            {
                gltf::AccessResult result = gltf::Access(GLTF, id);
                if (result.accessor->componentType == GL_FLOAT)
                {
                    VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, primitive->attribute.TEXCOORD_0);
                }
            }
            id = gPrimitive->Find("JOINTS_0");
            if (id != -1)
            {
                gltf::AccessResult result = gltf::Access(GLTF, id);
                if (result.accessor->componentType == GL_UNSIGNED_BYTE)
                {
                    vector<u8vec4> u8v;
                    VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, u8v);
                    vector<ivec4> iv(u8v.begin(), u8v.end());
                    primitive->attribute.JOINTS_0 = move(iv);
                }
            }
            id = gPrimitive->Find("WEIGHTS_0");
            if (id != -1)
            {
                gltf::AccessResult result = gltf::Access(GLTF, id);
                if (result.accessor->componentType == GL_FLOAT)
                {
                    VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, primitive->attribute.WEIGHTS_0);
                }
            }
            id = gPrimitive->indices;
            if (id != -1)
            {
                gltf::AccessResult result = gltf::Access(GLTF, id);
                if (result.accessor->componentType == GL_UNSIGNED_INT)
                {
                    VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, primitive->indices);
                }
                else if (result.accessor->componentType == GL_UNSIGNED_SHORT)
                {

                    vector<unsigned short> u16;
                    VectorFromFile(dir+result.buffer->uri, result.accessor->byteOffset+result.bufferView->byteOffset, result.accessor->count, u16);
                    vector<unsigned int> u32(u16.begin(), u16.end());
                    primitive->indices = move(u32);
                }
                else
                {
                    LOG("not find indices type !")
                }
            }
            if (gPrimitive->material != -1)
            {
                primitive->material = materials[gPrimitive->material];
            }
            else
            {
                primitive->material = make_shared<Material>();
                primitive->material->UseDefaultShader();
            }
            
            primitive->SetupGLPrimitive();
        }
        
    }
    void SetupNode(const gltf::glTF& GLTF, int i)
    {
        const gltf::Node* gNode = &GLTF.nodes[i];
        SceneNode* node = &nodes[i];

        node->name = gNode->name;
        if (gNode->hasMatrix())
        {
            mat4 matrix;
            memcpy(&matrix, &gNode->matrix, sizeof(mat4));
            node->localTransform = Transform(matrix.transpose());
        }
        if (gNode->hasTransform())
        {
            memcpy(&(node->localTransform.translation), &gNode->translation, sizeof(vec3));
            memcpy(&(node->localTransform.rotation), &gNode->rotation, sizeof(quat));
            memcpy(&(node->localTransform.scaling), &gNode->scale, sizeof(vec3));
        }
        node->childrenID = gNode->children;
        node->meshID = gNode->mesh;
        node->skinID = gNode->skin;
    }
    void SetupScene(const gltf::glTF& GLTF, int i)
    {
        const gltf::Scene* gScene = &GLTF.scenes[i];
        Scene* scene = &scenes[i];

        scene->name = gScene->name;
        scene->nodeIDs.resize(gScene->nodes.size());
        memcpy(&scene->nodeIDs[0], &gScene->nodes[0], sizeof(int)*gScene->nodes.size());
    }
};

Blob<Scenes> scenesBlob;

#endif