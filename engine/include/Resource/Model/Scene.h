#ifndef SCENE_H
#define SCENE_H

#include "Core/Log/Log.h"
#include "SDK/STL/STL.h"
#include "Core/Parse/JSONFileParse.h"
#include "Core/Parse/GLTFFileParse.h"
#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/Component.h"
#include "Resource/Model/Mesh.h"
#include "Resource/Material/Material.h"
#include "Resource/Texture/Texture2D.h"

class Scene
{
public:
    shared_ptr<Actor> root;
    vector<Actor*> nodes;
    vector<shared_ptr<Mesh>> meshs;
    vector<shared_ptr<Material>> materials;
    vector<shared_ptr<Texture2D>> texture2Ds;
    string path;
    string dir;
    void Load(const string &_path)
    {
        // path = _path;
        // size_t found = path.find_last_of('/');
        // dir = path.substr(0, found != string::npos ? found + 1 : 0);
        // gltf::glTF GLTF = gltf::Load(path);
        // gltf::Scene gScene = GLTF.scenes[GLTF.scene];
    
        // nodes.resize(GLTF.nodes.size());
        // materials.resize(GLTF.materials.size());
        // meshs.resize(GLTF.meshes.size());
        // texture2Ds.resize(GLTF.textures.size());

        // for (int i = 0; i < GLTF.textures.size(); i++)
        // {
        //     texture2Ds[i] = make_shared<Texture2D>();
        //     SetupTextures(GLTF, texture2Ds[i].get(), &GLTF.textures[i]);
        // }


        // for (int i = 0; i < GLTF.materials.size(); i++)
        // {
        //     materials[i] = make_shared<Material>();
        //     SetupMaterial(GLTF, materials[i].get(), &GLTF.materials[i]);
        // }

        // for (int i = 0; i < GLTF.meshes.size(); i++)
        // {
        //     meshs[i] = make_shared<Mesh>();
        //     SetupMesh(GLTF, meshs[i].get(), &GLTF.meshes[i]);
        // }
        

        // root = make_shared<Actor>();
        // root->name = gScene.name;
        // for (int i = 0; i < gScene.nodes.size(); i++)
        // {
        //     nodes[gScene.nodes[i]] = root->AddChild<Actor>();
        //     SetupNode(GLTF, nodes[gScene.nodes[i]], &GLTF.nodes[gScene.nodes[i]]);
        // }
    }
    void SetupTextures(const gltf::glTF& GLTF, Texture2D* texture, const gltf::Texture* gTexture)
    {
        texture->name = gTexture->name;
        texture->sampler = make_shared<TextureSampler>();
        if (gTexture->sampler != -1)
        {
            const gltf::Sampler* gSampler = &GLTF.samplers[gTexture->sampler];
            if (gSampler->magFilter != -1)
            {
                texture->sampler->magFilter = gSampler->magFilter;
            }
            if (gSampler->minFilter != -1)
            {
                texture->sampler->minFilter = gSampler->minFilter;
            }
            texture->sampler->wrapS = gSampler->wrapS;
            texture->sampler->wrapT = gSampler->wrapT;
        }
        texture->image = make_shared<Image>();
        if (gTexture->source != -1)
        {
            const gltf::Image* gImage = &GLTF.images[gTexture->source];
            texture->image->name = gImage->name;
            texture->image->Load(dir + gImage->uri);
        }
        else
        {
            LOG("filed to find gltf texture " << gTexture->name << " source !")
        }
        texture->SetupGLTexture2D();
    }
    void SetupMaterial(const gltf::glTF& GLTF, Material* material, const gltf::Material* gMaterial)
    {
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
    void SetupMesh(const gltf::glTF& GLTF, Mesh* mesh, const gltf::Mesh* gMesh)
    {
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
    void SetupNode(const gltf::glTF& GLTF, Actor* node, const gltf::Node* gNode)
    {
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
        for (int i = 0; i < gNode->children.size(); i++)
        {
            Actor* child = node->AddChild<Actor>();
            SetupNode(GLTF, child, &GLTF.nodes[gNode->children[i]]);
        }
    }
};

#endif