#ifndef MESH_H
#define MESH_H

#include "Core.h"
#include "GL.h"
#include "Image.h"
#include "AC.h"
#include "Transform.h"

struct AccessorResult
{
	string binPath;
	string type;
	int componentType;
	int count;
	int byteOffset;
};
void AccessorParse(const string &path, const json &GLTF, int accessorID, AccessorResult &result)
{
	json ACCESSORS = GLTF["accessors"];
	json ACCESSOR = ACCESSORS[accessorID];

	result.componentType = ACCESSOR["componentType"];
	result.count = ACCESSOR["count"];
	result.byteOffset = ACCESSOR.value("byteOffset", 0);
	result.type = ACCESSOR["type"];

	int bufferViewID = ACCESSOR["bufferView"];
	json BUFFER_VIEWS = GLTF["bufferViews"];
	json BUFFER_VIEW = BUFFER_VIEWS[bufferViewID];
	int bufferID = BUFFER_VIEW["buffer"];
	json BUFFERS = GLTF["buffers"];
	json BUFFER = BUFFERS[bufferID];

	result.byteOffset += BUFFER_VIEW.value("byteOffset", 0);

	string uri = BUFFER["uri"];
	string rootDir = path.substr(0, path.find_last_of("/")+1);

	result.binPath = rootDir + uri;
}

class Material
{
public:
};
class BoundingBox
{
public:
	vec3 min = vec3(0), max = vec3(0);
};
class MeshPrimitive
{
public:
	vector<vec3> position;
	vector<vec2> texcoord_0;
	vector<vec3> normal;
	vector<vec4> tangent;
	vector<ivec4> joints_0;
	vector<vec4> weights_0;
	vector<unsigned int> indices;
	shared_ptr<Material> material;
	BoundingBox boundingBox;
	void LoadFromGLTF(const string &path, const json GLTF, const json &PRIMITIVE)
	{
		json ATTRIBUTES = PRIMITIVE["attributes"];
		if (ATTRIBUTES.contains("POSITION"))
		{
			int accessorID = ATTRIBUTES["POSITION"];
			json ACCESSORS = GLTF["accessors"];
			json ACCESSOR = ACCESSORS[accessorID];
			
			if (ACCESSOR.contains("min"))
			{
				json MIN = ACCESSOR["min"];
				for (int i = 0; i < 3; i++)
				{
					boundingBox.min[i] = MIN[i];
				}
			}
			if (ACCESSOR.contains("max"))
			{
				json MAX = ACCESSOR["max"];
				for (int i = 0; i < 3; i++)
				{
					boundingBox.max[i] = MAX[i];
				}
			}

			AccessorResult result;
			AccessorParse(path, GLTF, accessorID, result);

			if (result.componentType == GL_FLOAT && result.type == "VEC3")
			{
				VectorFromFile(result.binPath, result.byteOffset, result.count, position);
			}
			else
			{
				LOG("accessor componentType not support !")
			}
		}
		if (ATTRIBUTES.contains("TEXCOORD_0"))
		{
			int accessorID = ATTRIBUTES["TEXCOORD_0"];

			AccessorResult result;
			AccessorParse(path, GLTF, accessorID, result);
			
			if (result.componentType == GL_FLOAT && result.type == "VEC2")
			{
				VectorFromFile(result.binPath, result.byteOffset, result.count, texcoord_0);
			}
			else
			{
				LOG("accessor componentType not support !")
			}
		}
		if (ATTRIBUTES.contains("NORMAL"))
		{
			int accessorID = ATTRIBUTES["NORMAL"];

			AccessorResult result;
			AccessorParse(path, GLTF, accessorID, result);
			
			if (result.componentType == GL_FLOAT && result.type == "VEC3")
			{
				VectorFromFile(result.binPath, result.byteOffset, result.count, normal);
			}
			else
			{
				LOG("accessor componentType not support !")
			}
		}
		if (ATTRIBUTES.contains("TANGENT"))
		{
			int accessorID = ATTRIBUTES["TANGENT"];

			AccessorResult result;
			AccessorParse(path, GLTF, accessorID, result);
			
			if (result.componentType == GL_FLOAT && result.type == "VEC4")
			{
				VectorFromFile(result.binPath, result.byteOffset, result.count, tangent);
			}
			else
			{
				LOG("accessor componentType not support !")
			}
		}
		if (ATTRIBUTES.contains("JOINTS_0"))
		{
			int accessorID = ATTRIBUTES["JOINTS_0"];

			AccessorResult result;
			AccessorParse(path, GLTF, accessorID, result);
			
			if (result.componentType == GL_UNSIGNED_BYTE && result.type == "VEC4")
			{
				vector<u8vec4> u8Joints;
				VectorFromFile(result.binPath, result.byteOffset, result.count, u8Joints);
				vector<ivec4> iJoints(begin(u8Joints), end(u8Joints));
				joints_0 = move(iJoints);
			}
			else
			{
				LOG("accessor componentType not support !")
			}
		}
		if (ATTRIBUTES.contains("WEIGHTS_0"))
		{
			int accessorID = ATTRIBUTES["WEIGHTS_0"];

			AccessorResult result;
			AccessorParse(path, GLTF, accessorID, result);
			
			if (result.componentType == GL_FLOAT && result.type == "VEC4")
			{
				VectorFromFile(result.binPath, result.byteOffset, result.count, weights_0);
			}
			else
			{
				LOG("accessor componentType not support !")
			}
		}
		if (PRIMITIVE.contains("indices"))
		{
			int accessorID = PRIMITIVE["indices"];

			AccessorResult result;
			AccessorParse(path, GLTF, accessorID, result);
			
			if (result.componentType == GL_UNSIGNED_SHORT && result.type == "SCALAR")
			{
				vector<unsigned short> u8Indices;
				VectorFromFile(result.binPath, result.byteOffset, result.count, u8Indices);
				vector<unsigned int> uIndices(begin(u8Indices), end(u8Indices));
				indices = move(uIndices);
			}
			else if(result.componentType == GL_UNSIGNED_INT && result.type == "SCALAR")
			{
				VectorFromFile(result.binPath, result.byteOffset, result.count, indices);
			}
			else
			{
				LOG("accessor componentType not support !")
			}
		}
		else
		{
			LOG("Load mesh Primitive failed, not find indices !")
		}
		if (PRIMITIVE.contains("material"))
		{
			
		}
		else
		{

		}
	}
};
class Mesh
{
public:
	string name;
	vector<MeshPrimitive> primitives;
	void LoadFromGLTF(const string &path, const json &GLTF, int meshID)
	{
		json MESHES = GLTF["meshes"];
		json MESH = MESHES[meshID];
		name = MESH.value("name", "");
		json PRIMITIVES = MESH["primitives"];
		primitives.resize(PRIMITIVES.size());
		for (int i = 0; i < PRIMITIVES.size(); i++)
		{
			json PRIMITIVE = PRIMITIVES.at(i);
			primitives[i].LoadFromGLTF(path, GLTF, PRIMITIVE);
		}
	}
};
namespace ResourcePool
{
	map<string, weak_ptr<Mesh>> meshMap;
	shared_ptr<Mesh> GetMesh(const string &path, const json &GLTF, int meshID)
	{
		string key = path + ":" + to_string(meshID);
		map<string, weak_ptr<Mesh>>::iterator found = meshMap.find(key);
		if (found != meshMap.end() && found->second.use_count() != 0)
		{
			return found->second.lock();
		}
		else
		{
			shared_ptr<Mesh> added = make_shared<Mesh>();
			meshMap[key] = added;
			added->LoadFromGLTF(path, GLTF, meshID);
			return added;
		}
	}
}
class Skin
{
public:
	vector<mat4> inverseBindMatrices;
	vector<int> joints;
	void LoadFromGLTF(const string &path, const json &GLTF, int skinID)
	{
		json SKINS = GLTF["skins"];
		json SKIN = SKINS[skinID];
		json JOINTS = SKIN["joints"];
		joints.resize(JOINTS.size());
		for (int i = 0; i < JOINTS.size(); i++)
		{
			joints[i] = JOINTS[i];
		}

		if (SKIN.contains("inverseBindMatrices"))
		{
			int inverseBindMatricesID = SKIN["inverseBindMatrices"];
		
			AccessorResult result;
			AccessorParse(path, GLTF, inverseBindMatricesID, result);
			if (result.componentType == GL_FLOAT && result.type == "MAT4")
			{
				VectorFromFile<mat4>(result.binPath, result.byteOffset, result.count, inverseBindMatrices);
			}
			else
			{
				LOG("accessor componentType not support !")
			}
		}
		else
		{
			LOG("failed read inverseBindMatrices !")
		}
		
	}
};
namespace ResourcePool
{
	map<string, weak_ptr<Skin>> skinMap;
	shared_ptr<Skin> GetSkin(const string &path, const json &GLTF, int skinID)
	{
		string key = path + ":" + to_string(skinID);
		map<string, weak_ptr<Skin>>::iterator found = skinMap.find(key);
		if (found != skinMap.end() && found->second.use_count() != 0)
		{
			return found->second.lock();
		}
		else
		{
			shared_ptr<Skin> added = make_shared<Skin>();
			skinMap[key] = added;
			added->LoadFromGLTF(path, GLTF, skinID);
			return added;
		}
	}
};
class AnimationChannelTarget
{
public:
	int nodeID;
	string path;
	void LoadFromGLTF(const string &gltfPath, const json &GLTF, const json &TARGET)
	{
		if (TARGET.contains("node"))
		{
			nodeID = TARGET["node"];
		}
		else
		{
			LOG("Animation channel target failed to find node !")
		}
		path = TARGET["path"];
	}
};
class AnimationSampler
{
public:
	vector<float> input;
	string interpolation;
	vector<quat> outputQuat;
	vector<vec3> outputVec3;
	float min, max;
	void LoadFromGLTF(const string &path, const json &GLTF, const json &SAMPLER)
	{
		interpolation = SAMPLER.value("interpolation", "");
		int inputID = SAMPLER["input"];

		json ACCESSORS = GLTF["accessors"];
		json ACCESSOR = ACCESSORS[inputID];
		if (ACCESSOR.contains("min"))
		{
			json MIN = ACCESSOR["min"];
			min = MIN[0];
		}
		if (ACCESSOR.contains("max"))
		{
			json MAX = ACCESSOR["max"];
			max = MAX[0];
		}

		AccessorResult result;
		AccessorParse(path, GLTF, inputID, result);
		VectorFromFile(result.binPath, result.byteOffset, result.count, input);

		int outputID = SAMPLER["output"];
		AccessorParse(path, GLTF, outputID, result);
		if (result.componentType == GL_FLOAT && result.type == "VEC3")
		{
			VectorFromFile(result.binPath, result.byteOffset, result.count, outputVec3);
		}
		else if (result.componentType == GL_FLOAT && result.type == "VEC4")
		{
			VectorFromFile(result.binPath, result.byteOffset, result.count, outputQuat);
		}
		else
		{
			LOG("failed to read AnimationSampler, output type doesn't support !");
		}

	}
	vec3 SampleVec3(float time)
	{
		return vec3(1.);
	}
	quat SampleQuat(float time)
	{
		return quat();
	}
};
class Animation;
class AnimationChannel
{
public:
	int samplerID;
	AnimationSampler* sampler;
	Animation* upperAnimation;
	AnimationChannelTarget target;
	
	void LoadFromGLTF(const string &path, const json &GLTF, const json &CHANNEL, Animation* animation, vector<AnimationSampler> &samplers)
	{
		samplerID = CHANNEL["sampler"];
		json TARGET = CHANNEL["target"];
		target.LoadFromGLTF(path, GLTF, TARGET);
		sampler = &samplers[samplerID];
		upperAnimation = animation;
	}
};
class Animation
{
public:
	string name;
	vector<AnimationChannel> channels;
	vector<AnimationSampler> samplers;
	float min, max;
	void LoadFromGLTF(const string &path, const json &GLTF, int animationID)
	{
		json ANIMATIONS = GLTF["animations"];
		json ANIMATION = ANIMATIONS[animationID];
		name = ANIMATION.value("name", "");
		
		json SAMPLERS = ANIMATION["samplers"];
		samplers.resize(SAMPLERS.size());
		for (int i = 0; i < SAMPLERS.size(); i++)
		{
			json SAMPLER = SAMPLERS[i];
			samplers[i].LoadFromGLTF(path, GLTF, SAMPLER);
			if (i == 0)
			{
				min = samplers[i].min;
				max = samplers[i].max;
			}
			else
			{
				min = std::min(min, samplers[i].min);
				max = std::max(max, samplers[i].max);
			}
		}

		json CHANNELS = ANIMATION["channels"];
		channels.resize(CHANNELS.size());
		for (int i = 0; i < CHANNELS.size(); i++)
		{
			json CHANNEL = CHANNELS[i];
			channels[i].LoadFromGLTF(path, GLTF, CHANNEL, this, samplers);
		}
	}
};
namespace ResourcePool
{
	map<string, weak_ptr<Animation>> animationMap;
	shared_ptr<Animation> GetAnimation(const string &path, const json &GLTF, int animationID)
	{
		string key = path + ":" + to_string(animationID);
		map<string, weak_ptr<Animation>>::iterator found = animationMap.find(key);
		if (found != animationMap.end() && found->second.use_count() != 0)
		{
			return found->second.lock();
		}
		else
		{
			shared_ptr<Animation> added = make_shared<Animation>();
			animationMap[key] = added;
			added->LoadFromGLTF(path, GLTF, animationID);
			return added;
		}
	}
};
class MeshComponent : public Component
{
public:
	shared_ptr<Mesh> mesh;
};
class SkinComponent : public Component
{
public:
	shared_ptr<Skin> skin;
	vector<Actor*>* nodes;
};
class ModelComponent;
class AnimationChannelsComponent : public Component
{
public:
	vector<AnimationChannel*> animationChannels;
	vector<float*> animationChannelWeights;
	ModelComponent* upperModelComponent;
};
class ModelComponent : public Component
{
public:
	vector<shared_ptr<Animation>> animations;
	vector<float> animationWeights;
	string currentAnimationName;

	vector<Actor*> nodes;


	void Load(const string &path)
	{
		Actor *parent = (Actor*)owner;
		
		json GLTF = json::parse(ifstream(path));

		if (GLTF.contains("nodes"))
		{
			json NODES = GLTF["nodes"];
			nodes.resize(NODES.size());
		}
		
		if (GLTF.contains("animations"))
		{
			json ANIMATIONS = GLTF["animations"];
			animations.resize(ANIMATIONS.size());
			animationWeights.resize(ANIMATIONS.size());
			for (int i = 0; i < ANIMATIONS.size(); i++)
			{
				animations[i] = ResourcePool::GetAnimation(path, GLTF, i);
				animationWeights[i] = 0;
			}
		}
		
		if (GLTF.contains("scene"))
		{
			int sceneID = GLTF["scene"];
			json SCENES = GLTF["scenes"];
			json SCENE = SCENES[sceneID];

			name = SCENE.value("name", "");
			parent->name = name;
			if (SCENE.contains("nodes"))
			{
				json SCENE_NODES = SCENE["nodes"];
				for (int i = 0; i < SCENE_NODES.size(); i++)
				{
					int nodeID = SCENE_NODES.at(i);
					LoadNodeRecursively(parent, path, GLTF, nodeID);
				}
			}
		}

		for (int i = 0; i < animations.size(); i++)
		{
			for (int j = 0; j < animations[i]->channels.size(); j++)
			{
				AnimationChannel* channel = &(animations[i]->channels[j]);
				int nodeID = channel->target.nodeID;
				Actor* node = nodes[nodeID];

				AnimationChannelsComponent* channelsComponent = node->GetComponent<AnimationChannelsComponent>();
				if (channelsComponent == nullptr)
				{
					channelsComponent = node->AddComponent<AnimationChannelsComponent>();
					channelsComponent->upperModelComponent = this;
				}

				channelsComponent->animationChannels.push_back(channel);
				channelsComponent->animationChannelWeights.push_back(&animationWeights[i]);
			}
		}
		
	}
	void LoadNodeRecursively(Actor *parent, const string &path, const json &GLTF, int nodeID)
	{
		json NODES = GLTF["nodes"];
		json NODE = NODES[nodeID];
		Actor* added = parent->AddChild<Actor>();
		nodes[nodeID] = added;
		added->name = NODE.value("name", "");

		if (NODE.contains("translation"))
		{
			for (int i = 0; i < 3; i++)
			{
				added->localTransform.translation[i] = NODE["translation"][i];
			}
		}
		if (NODE.contains("rotation"))
		{
			for (int i = 0; i < 4; i++)
			{
				added->localTransform.rotation[i] = NODE["rotation"][i];
			}
		}
		if (NODE.contains("scale"))
		{
			for (int i = 0; i < 3; i++)
			{
				added->localTransform.scale[i] = NODE["scale"][i];
			}
		}
		if (NODE.contains("matrix"))
		{
			mat4 nodeMatrix;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					nodeMatrix[i][j] = NODE["matrix"][i*4 + j];
				}
			}
			added->localTransform = Transform(nodeMatrix);
		}

		if (NODE.contains("mesh"))
		{
			int meshID = NODE["mesh"];
			shared_ptr<Mesh> mesh = ResourcePool::GetMesh(path, GLTF, meshID);
			MeshComponent* meshComponent = added->AddComponent<MeshComponent>();
			meshComponent->mesh = mesh;
		}
		
		if (NODE.contains("skin"))
		{
			int skinID = NODE["skin"];
			shared_ptr<Skin> skin = ResourcePool::GetSkin(path, GLTF, skinID);
			SkinComponent* skinComponent = added->AddComponent<SkinComponent>();
			skinComponent->nodes = &nodes;
			skinComponent->skin = skin;
		}
		
		if (NODE.contains("children"))
		{
			json CHILDREN = NODE["children"];
			for (int i = 0; i < CHILDREN.size(); i++)
			{
				int childrenID = CHILDREN.at(i);
				LoadNodeRecursively(added, path, GLTF, childrenID);
			}
		}
	}
};

#endif