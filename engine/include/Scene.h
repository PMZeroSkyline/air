#ifndef SCENE_H
#define SCENE_H

#include "Core.h"
#include "GL.h"
#include "AC.h"
#include "GLTF.h"
#include "Blob.h"


class Sampler
{
public:
    int magFilter = GL_LINEAR;
    int minFilter = GL_LINEAR;
    int wrapS = GL_REPEAT;
    int wrapT = GL_REPEAT;
};
class Texture2D
{
public:
    string name;
    Sampler sampler;
    shared_ptr<Image> image;
    GLTexture2D glTexture2D;
    void SetupGLTexture2D()
    {
        LOG("SetupGLTexture2D " << name)
        GLenum format = GL_RGBA;
        if (image->n == 1)
        {
            format = GL_RED;
        }
        if (image->n == 3)
        {
            format = GL_RGB;
        }
        glTexture2D.Setup(sampler.wrapS, sampler.wrapT, sampler.minFilter, sampler.magFilter, format, image->w, image->h, format, GL_UNSIGNED_BYTE, image->d);
    }
};
class Shader
{
public:
    string name;
    GLProgram glProgram;
    void Load(const string &vsPath, const string &fsPath)
    {
        name = "vs:" + vsPath + ",fs:" + fsPath;
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
        glUniform1i(glGetUniformLocation(glProgram.id, name.c_str()), (int)value); 
    }
    void SetInt(const string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(glProgram.id, name.c_str()), value); 
    }
    void SetFloat(const string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(glProgram.id, name.c_str()), value); 
    }
    void SetVec2(const string &name, const vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(glProgram.id, name.c_str()), 1, &value[0]); 
    }
    void SetVec2(const string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(glProgram.id, name.c_str()), x, y); 
    }
    void SetVec3(const string &name, const vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(glProgram.id, name.c_str()), 1, &value[0]); 
    }
    void SetVec3(const string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(glProgram.id, name.c_str()), x, y, z); 
    }
    void SetVec4(const string &name, const vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(glProgram.id, name.c_str()), 1, &value[0]); 
    }
    void SetVec4(const string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(glProgram.id, name.c_str()), x, y, z, w); 
    }
    void SetMat2(const string &name, const mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(glProgram.id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void SetMat3(const string &name, const mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(glProgram.id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void SetMat4(const string &name, const mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(glProgram.id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};
Blob<Shader> shaderBlob;
class Material
{
public:
    int layer;
    string name;
    shared_ptr<Shader> shader;
    vector<pair<string, shared_ptr<Texture2D>>> texturePairs;
    void UseDefaultShader()
    {
        shader = shaderBlob.Get("default");
        if (!shader)
        {
            shader = make_shared<Shader>();
            shader->Load("shader/default_vs.glsl", "shader/default_fs.glsl");
            shaderBlob.Set("default", shader);
        }
    }
    void Setup()
    {
        for (int i = 0; i < texturePairs.size(); i++)
        {
            shader->SetInt(texturePairs[i].first, i);
        }
    }
    void Bind()
    {
        for (int i = 0; i < texturePairs.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, texturePairs[i].second->glTexture2D.id);
        }
    }
};
class Bone
{
public:
    string name;
    mat4 inverseBindPoseMatrix;
    Actor* actor;
};
class Armature
{
public:
    vector<Bone> bones;
    string GetBonesName()
    {
        string names;
        for_each(bones.begin(), bones.end(), [&names](Bone &bone){names += bone.name + " ";});
        return names;
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
    vector<vec4> COLOR_0;
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
    shared_ptr<Material> material;
    shared_ptr<Armature> armature;
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
class AnimationChannel
{
public:
    string targetName;
    vector<pair<float, vec3>> translationPairs;
	vector<pair<float, quat>> rotationPairs;
	vector<pair<float, vec3>> scalePairs;
    template<typename T>
    bool GetInterpStep(const vector<pair<float, T>> &input, float time, int &outBeg, int &outEnd, float &outInterp)
    {
        if (input.size() == 0)
        {
            return false;
        }
        for (int i = 0; i < input.size(); i++)
        {
            const float &keyTime = input[i].first;
            if (keyTime == time)
            {
                outBeg = i;
                outEnd = i;
                outInterp = 0;
                return true;
            }
            else if (keyTime > time)
            {
                if (i == 0)
                {
                    return false;
                }
                else
                {
                    outBeg = i - 1;
                    outEnd = i;
                    float a = input[outBeg].first;
                    float b = input[outEnd].first;
                    outInterp = (time - a) / (b - a);
                }
            }
        }
        int endI = input.size()-1;
        outBeg = endI;
        outEnd = endI;
        outInterp = endI;
        return true;
    }
    vec3 SampleTranslation(float time)
	{
        int beg, end;
        float interp;
        if (GetInterpStep(translationPairs, time, beg, end, interp))
        {
            if (interp == 0)
            {
                return translationPairs[beg].second;
            }
            else if(interp == 1)
            {
                return translationPairs[end].second;
            }
            else
            {
                vec3 a = translationPairs[beg].second;
                vec3 b = translationPairs[end].second;
                return mix(a, b, interp);
            }
        }
        else
        {
            return vec3(0.f);
        }
	}
    vec3 SampleScale(float time)
	{
        int beg, end;
        float interp;
        if (GetInterpStep(scalePairs, time, beg, end, interp))
        {
            if (interp == 0)
            {
                return scalePairs[beg].second;
            }
            else if(interp == 1)
            {
                return scalePairs[end].second;
            }
            else
            {
                vec3 a = scalePairs[beg].second;
                vec3 b = scalePairs[end].second;
                return mix(a, b, interp);
            }
        }
        else
        {
            return vec3(1.f);
        }
	}
	quat SampleRotation(float time)
	{
		int beg, end;
        float interp;
        if (GetInterpStep(rotationPairs, time, beg, end, interp))
        {
            if (interp == 0)
            {
                return rotationPairs[beg].second;
            }
            else if(interp == 1)
            {
                return rotationPairs[end].second;
            }
            else
            {
                quat a = rotationPairs[beg].second;
                quat b = rotationPairs[end].second;
                return slerp(a, b, interp);
            }
        }
        else
        {
            return quat();
        }
	}
};
class Animation
{
public:
    string name;
    vector<shared_ptr<AnimationChannel>> channels;
};
class MeshComponent : public Component
{
public:
    shared_ptr<Mesh> mesh;
};
class Scene
{
public:
    string name;
    string dir;
    string path;
    unique_ptr<Actor> root;
    vector<shared_ptr<Material>> materials;
    vector<shared_ptr<Mesh>> meshs;
    vector<shared_ptr<Armature>> armatures;
    vector<shared_ptr<Animation>> animations;

    void Load(const string &_path)
    {
        path = _path;
        dir = path.substr(0, path.find_last_of('/') == string::npos ? 0 : path.find_last_of('/') + 1);

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenBoundingBoxes);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        name = scene->mName.C_Str();

        if (scene->HasMaterials())
        {
            materials.resize(scene->mNumMaterials);
            for (int i = 0; i < scene->mNumMaterials; i++)
            {
                materials[i] = make_shared<Material>();
                SetupMaterial(scene->mMaterials[i], materials[i].get());
            }
        }

        root = make_unique<Actor>();
        SetupNodes(scene, scene->mRootNode, root.get());

        if (scene->HasAnimations())
        {
            animations.resize(scene->mNumAnimations);
            for (int i = 0; i < scene->mNumAnimations; i++)
            {
                animations[i] = make_shared<Animation>();
                SetupAnimation(scene->mAnimations[i], animations[i].get());
            }
        }
    }
    void SetupNodes(const aiScene* scene, const aiNode *aNode, Actor *node)
    {
        node->name = aNode->mName.C_Str();
        if (aNode->mNumMeshes != 0)
        {
            shared_ptr<Mesh> mesh = make_shared<Mesh>();
            meshs.push_back(mesh);
            MeshComponent* meshComponent = node->AddComponent<MeshComponent>();
            meshComponent->mesh = mesh;
            mesh->primitives.resize(aNode->mNumMeshes);
            mesh->name = aNode->mName.C_Str();
            shared_ptr<Armature> usedArmature;
            for (int i = 0; i < aNode->mNumMeshes; i++)
            {
                SetupMeshPrimitive(scene->mMeshes[aNode->mMeshes[i]], &mesh->primitives[i]);
            }
        }
        for (int i = 0; i < aNode->mNumChildren; i++)
        {
            SetupNodes(scene, aNode->mChildren[i], node->AddChild<Actor>());
        }
    }
    void SetupMaterial(const aiMaterial* aMaterial, Material* material)
    {
        
    }
    void SetupMeshPrimitive(const aiMesh* aMesh, MeshPrimitive* primitive)
    {
        if (aMesh->HasPositions())
        {
            primitive->attribute.POSITION.resize(aMesh->mNumVertices);
            memcpy(&primitive->attribute.POSITION[0], aMesh->mVertices, sizeof(vec3)*aMesh->mNumVertices);
        }
        if (aMesh->HasNormals())
        {
            primitive->attribute.NORMAL.resize(aMesh->mNumVertices);
            memcpy(&primitive->attribute.NORMAL[0], aMesh->mNormals, sizeof(vec3)*aMesh->mNumVertices);
        }
        if (aMesh->HasTangentsAndBitangents())
        {
            primitive->attribute.TANGENT.resize(aMesh->mNumVertices);
            Read<vec4, vec3>(&primitive->attribute.TANGENT[0], sizeof(vec3), aMesh->mNumVertices, (vec3*)aMesh->mTangents);
            for_each(primitive->attribute.TANGENT.begin(), primitive->attribute.TANGENT.end(), [](vec4 &item){item.w = 1.f;});
        }
        if (aMesh->HasTextureCoords(0))
        {
            primitive->attribute.TEXCOORD_0.resize(aMesh->mNumVertices);
            Read<vec2, vec3>(&primitive->attribute.TEXCOORD_0[0], sizeof(vec2), aMesh->mNumVertices, (vec3*)aMesh->mTextureCoords[0]);
        }
        if (aMesh->HasTextureCoords(1))
        {
            primitive->attribute.TEXCOORD_1.resize(aMesh->mNumVertices);
            Read<vec2, vec3>(&primitive->attribute.TEXCOORD_1[0], sizeof(vec2), aMesh->mNumVertices, (vec3*)aMesh->mTextureCoords[1]);
        }
        if (aMesh->HasTextureCoords(2))
        {
            primitive->attribute.TEXCOORD_2.resize(aMesh->mNumVertices);
            Read<vec2, vec3>(&primitive->attribute.TEXCOORD_2[0], sizeof(vec2), aMesh->mNumVertices, (vec3*)aMesh->mTextureCoords[2]);
        }
        if (aMesh->HasTextureCoords(3))
        {
            primitive->attribute.TEXCOORD_3.resize(aMesh->mNumVertices);
            Read<vec2, vec3>(&primitive->attribute.TEXCOORD_3[0], sizeof(vec2), aMesh->mNumVertices, (vec3*)aMesh->mTextureCoords[3]);
        }
        if (aMesh->HasVertexColors(0))
        {
            primitive->attribute.COLOR_0.resize(aMesh->mNumVertices);
            memcpy(&primitive->attribute.COLOR_0[0], aMesh->mColors[0], sizeof(vec4)*aMesh->mNumVertices);
        }
        if (aMesh->HasBones())
        {
            vector<int> jointis;
            primitive->attribute.JOINTS_0.resize(aMesh->mNumVertices);
            primitive->attribute.WEIGHTS_0.resize(aMesh->mNumVertices);
            jointis.resize(aMesh->mNumVertices);
            string boneNames;
            for (int boneId = 0; boneId < aMesh->mNumBones; boneId++)
            {
                aiBone* aBone = aMesh->mBones[boneId];
                boneNames += aBone->mName.C_Str();
                boneNames += " ";
                for (int weightId = 0; weightId < aBone->mNumWeights; weightId++)
                {
                    aiVertexWeight* aVertWeight = &aBone->mWeights[weightId];
                    int vid = aVertWeight->mVertexId;
                    int &vi = jointis[vid];
                    if (vi < 4)
                    {
                        primitive->attribute.JOINTS_0[vid][vi] = boneId;
                        primitive->attribute.WEIGHTS_0[vid][vi] = aVertWeight->mWeight;
                        vi++;
                    }
                    else
                    {
                        LOG("bone weight over 4 " << aBone->mName.C_Str() << " vid:" << vid << " ?")
                    }
                }
            }

            vector<shared_ptr<Armature>>::reverse_iterator foundArmature = find_if(armatures.rbegin(), armatures.rend(), [&boneNames](shared_ptr<Armature> &armature){return armature->GetBonesName() == boneNames;});
            if (foundArmature != armatures.rend())
            {
                primitive->armature = *foundArmature;
            }
            else
            {
                shared_ptr<Armature> armature = make_shared<Armature>();
                armature->bones.resize(aMesh->mNumBones);
                for (int boneId = 0; boneId < aMesh->mNumBones; boneId++)
                {
                    aiBone* aBone = aMesh->mBones[boneId];
                    Bone* bone = &armature->bones[boneId];
                    bone->name = aBone->mName.C_Str();
                    memcpy(&bone->inverseBindPoseMatrix, &aBone->mOffsetMatrix, sizeof(mat4));
                }
                primitive->armature = armature;
                armatures.push_back(armature);
            }
        }
        
        if (aMesh->HasFaces())
        {
            primitive->indices.resize(aMesh->mNumFaces * 3);
            for (int faceId = 0; faceId < aMesh->mNumFaces; faceId++)
            {
                aiFace* aFace = &aMesh->mFaces[faceId];
                if (aFace->mNumIndices != 3)
                {
                    LOG("mesh face indices size not equal 3 !")
                }
                else
                {
                    memcpy(&primitive->indices[faceId*3], aFace->mIndices, sizeof(unsigned int)*3);
                }
            }
        }
        else
        {
            LOG("mesh indices is null !")
        }
        memcpy(&primitive->boundingBox.min, &aMesh->mAABB.mMin, sizeof(vec3));
        memcpy(&primitive->boundingBox.max, &aMesh->mAABB.mMax, sizeof(vec3));
    }
    void SetupAnimation(const aiAnimation* aAnimation, Animation* animation)
    {
        animation->name = aAnimation->mName.C_Str();
        animation->channels.resize(aAnimation->mNumChannels);
        for (int channelId = 0; channelId < aAnimation->mNumChannels; channelId++)
        {
            aiNodeAnim* aChannel = aAnimation->mChannels[channelId];
            shared_ptr<AnimationChannel> animationChannel = make_shared<AnimationChannel>();
            animation->channels[channelId] = animationChannel;
            animationChannel->targetName = aChannel->mNodeName.C_Str();
            animationChannel->translationPairs.resize(aChannel->mNumPositionKeys);
            for (int i = 0; i < aChannel->mNumPositionKeys; i++)
            {
                aiVectorKey *aVectorKey = &aChannel->mPositionKeys[i];
                vec3 value; memcpy(&value, &aVectorKey->mValue, sizeof(vec3));
                animationChannel->translationPairs[i] = make_pair(aVectorKey->mTime, value);
            }
            animationChannel->rotationPairs.resize(aChannel->mNumRotationKeys);
            for (int i = 0; i < aChannel->mNumRotationKeys; i++)
            {
                aiQuatKey *aQuatKey = &aChannel->mRotationKeys[i];
                quat value; memcpy(&value, &aQuatKey->mValue, sizeof(quat));
                animationChannel->rotationPairs[i] = make_pair(aQuatKey->mTime, value);
            }
            animationChannel->scalePairs.resize(aChannel->mNumScalingKeys);
            for (int i = 0; i < aChannel->mNumScalingKeys; i++)
            {
                aiVectorKey *aVectorKey = &aChannel->mScalingKeys[i];
                vec3 value; memcpy(&value, &aVectorKey->mValue, sizeof(vec3));
                animationChannel->scalePairs[i] = make_pair(aVectorKey->mTime, value);
            }
        }
    }
};
Blob<Scene> sceneBlob;
class SceneComponent : public Component
{
    shared_ptr<Scene> scene;
    void Load(const string& path)
    {
        scene = sceneBlob.Get(path);
        if (!scene)
        {
            scene = make_shared<Scene>();
            scene->Load(path);
            sceneBlob.Set(path, scene);
        }
    }
    void FieldExpand(Actor* parent, Actor* src)
    {
        //Actor* des = owner->AddChild<Actor>();
        //*des = *src;
    }
};
shared_ptr<MeshPrimitive> MakeQuad()
{
    shared_ptr<MeshPrimitive> quad = make_shared<MeshPrimitive>();
    quad->attribute.POSITION = {
        {1.0f,  1.0f, 0.0f},
        {1.0f, -1.0f, 0.0f},
        {-1.0f, -1.0f, 0.0f},
        {-1.0f,  1.0f, 0.0f}
    };
    quad->attribute.TEXCOORD_0 = {
        {1.0f,  1.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f,  1.0f}
    };
    quad->indices = {
        0, 1, 3,   
        1, 2, 3    
    };
    quad->SetupGLPrimitive();
    quad->material = make_shared<Material>();
    quad->material->UseDefaultShader();
    return quad;
}
#endif