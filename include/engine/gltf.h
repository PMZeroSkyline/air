#ifndef GLTF_H
#define GLTF_H
#include <vector>
using std::vector;
#include <string>
#include <fstream>
using std::string;
using std::pair;
using std::ifstream;
#include "json/json.hpp"
using namespace nlohmann;
namespace gltf{
struct Accessor
{
	int bufferView;
	int byteOffset = 0;
	int componentType;
	bool normalized = false;
	int count;
	string type;
	vector<float> min;
	vector<float> max;
	json extensions;
	json extras;
	Accessor(int _componentType, int _count, const string &_type) : componentType(_componentType), count(_count), type(_type){}
};
struct AnimationChannelTarget
{
	int node;
	string path;
	json extension;
	json extras;
	AnimationChannelTarget(const string _path) : path(_path){}
};
struct AnimationChannel
{
	int sampler;
	AnimationChannelTarget target;
	json extension;
	json extras;
	AnimationChannel(int _sampler, const AnimationChannelTarget &_target) : sampler(_sampler), target(_target){}
};
struct AnimationSampler
{
	int input;
	string interpolation = "LINEAR";
	int output;
	json extension;
	json extras;
	AnimationSampler(int _input, int _output) : input(_input), output(_output){}
};
struct Animation
{
	vector<AnimationChannel> channels;
	vector<AnimationSampler> samplers;
	string name;
	json extensions;
	json extras;
	Animation(const vector<AnimationChannel> &_channels, const vector<AnimationSampler> &_samplers) : channels(_channels), samplers(_samplers){}
};
struct Asset
{
	string copyright;
	string generator;
	string version;
	string minVersion;
	json extensions;
	json extras;
	Asset(const string &_version) : version(_version){}
};
struct Buffer
{
	string uri;
	int byteLength;
	string name;
	json extensions;
	json extras;
	Buffer(int _byteLength) : byteLength(_byteLength){}
};
struct BufferView
{
	int buffer;
	int byteOffset = 0;
	int byteLength;
	int byteStride;
	int target;
	string name;
	json extensions;
	json extras;
	BufferView(int _buffer, int _byteLength) : buffer(_buffer), byteLength(_byteLength){}
};
struct CameraOrthographic
{
	float xmag;
	float ymag;
	float zfar;
	float znear;
	json extensions;
	json extras;
	CameraOrthographic(float _xmag, float _ymag, float _zfar, float _znear) : xmag(_xmag), ymag(_ymag), zfar(_zfar), znear(_znear) {}
};
struct CameraPerspective
{
	float aspectRatio;
	float yfov;
	float zfar;
	float znear;
	json extensions;
	json extras;
	CameraPerspective(float _yfov, float _znear) : yfov(_yfov), znear(_znear){}
};
struct Camera
{
	CameraOrthographic orthographic;
	CameraPerspective perspective;
	string type;
	json extensions;
	json extras;
};
struct Image
{
	string uri;
	string mimeType;
	int bufferView;
	string name;
	json extensions;
	json extras;
};
struct TextureInfo
{
	int index;
	int texCoord = 0;
	json extensions;
	json extras;
	TextureInfo(){index = -1;}
	TextureInfo(int _index) : index(_index){}
};
struct MaterialPBRMetallicRoughness
{
	float baseColorFactor[4] = {1,1,1,1};
	TextureInfo baseColorTexture;
	float metallicFactor = 1;
	float roughnessFactor = 1;
	TextureInfo metallicRoughnessTexture;
	json extensions;
	json extras;
};
struct MaterialNormalTextureInfo
{
	int index;
	int texCoord = 0;
	float scale = 1;
	json extensions;
	json extras;
	MaterialNormalTextureInfo(){index = -1;}
	MaterialNormalTextureInfo(int _index) : index(_index){}
};
struct MaterialOcclusionTextureInfo
{
	int index;
	int texCoord = 0;
	float strength = 1;
	json extensions;
	json extras;
	MaterialOcclusionTextureInfo(){index = -1;};
	MaterialOcclusionTextureInfo(int _index) : index(_index){}
};
struct Material
{
	string name;
	json extensions;
	json extras;
	MaterialPBRMetallicRoughness pbrMetallicRoughness;
	MaterialNormalTextureInfo normalTexture;
	MaterialOcclusionTextureInfo occlusionTexture;
	TextureInfo emissiveTexture;
	float emissiveFactor[3] = {0,0,0};
	string alphaMode = "OPAQUE";
	float alphaCutoff = 0.5;
	bool doubleSided = false;
};
struct MeshPrimitive
{
	vector<pair<string, int>> attributes;
	int indices;
	int material;
	int mode = 4;
	vector<json> targets;
	json extensions;
	json extras;
	MeshPrimitive(const vector<pair<string, int>> _attributes) : attributes(_attributes){}
};
struct Mesh
{
	vector<MeshPrimitive> primitives;
	vector<float> weights;
	string name;
	json extensions;
	json extras;
	Mesh(const vector<MeshPrimitive> _primitives) : primitives(_primitives){}
};
struct Node
{
	int camera;
	vector<int> children;
	int skin;
	float matrix[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
	int mesh;
	float rotation[4] = {0,0,0,1};
	float scale[3] = {1,1,1};
	float translation[3] = {0,0,0};
	vector<float> weights;
	string name;
	json extensions;
	json extras;
};
struct Sampler
{
	int magFilter;
	int minFilter;
	int wrapS = 10497;
	int wrapT = 10497;
	string name;
	json extensions;
	json extras;
};
struct Scene
{
	vector<int> nodes;
	string name;
	json extensions;
	json extras;
};
struct Skin
{
	int inverseBindMatrices;
	int skeleton;
	vector<int> joints;
	string name;
	json extensions;
	json extras;
	Skin(const vector<int> &_joints) : joints(_joints){}
};
struct Texture
{
	int sampler;
	int source;
	string name;
	json extensions;
	json extras;
};
struct glTF
{
	vector<string> extensionsUsed;
	vector<string> extensionsRequired;
	vector<Accessor> accessors;
	vector<Animation> animations;
	Asset asset;
	vector<Buffer> buffers;
	vector<BufferView> bufferViews;
	vector<Camera> cameras;
	vector<Image> images;
	vector<Material> materials;
	vector<Mesh> meshes;
	vector<Node> nodes;
	vector<Sampler> samplers;
	int scene;
	vector<Scene> scenes;
	vector<Skin> skins;
	vector<Texture> textures;
	json extensions;
	json extras;
	glTF(const Asset &_asset) : asset(_asset){}
};

glTF load(const string &path)
{
	json GLTF = json::parse(ifstream(path));
	json ASSET = GLTF["asset"];
	string version = ASSET["version"];
	Asset asset(version);
	glTF gltf(asset);
	if (GLTF.contains("extensionsUsed"))
	{
		json EXTENSIONS_USED = GLTF["extensionsUsed"];
		for (int i = 0; i < EXTENSIONS_USED.size(); i++)
		{
			gltf.extensionsUsed.push_back(EXTENSIONS_USED.at(i));
		}
	}
	gltf.scene = GLTF.value("scene", -1);
	if (GLTF.contains("scenes"))
	{
		json SCENES = GLTF["scenes"];
		for (int i = 0; i < SCENES.size(); i++)
		{
			json SCENE = SCENES.at(i);
			Scene scene;
			scene.name = SCENE.value("name","");
			if (SCENE.contains("nodes"))
			{
				json NODES = SCENE["nodes"];
				for (int j = 0; j < NODES.size(); j++)
				{
					scene.nodes.push_back(NODES.at(j));
				}
			}
			gltf.scenes.push_back(scene);
		}
	}
	if (GLTF.contains("nodes"))
	{
		json NODES = GLTF["nodes"];
		for (int i = 0; i < NODES.size(); i++)
		{
			json NODE = NODES.at(i);
			Node node;
			node.camera = NODE.value("camera", -1);
			if (NODE.contains("children"))
			{
				json CHILDREN = NODE["children"];
				for (int j = 0; j < CHILDREN.size(); j++)
				{
					node.children.push_back(CHILDREN.at(j));
				}
			}
			if (NODE.contains("matrix"))
			{
				json MATRIX = NODE["matrix"];
				for (int j = 0; j < MATRIX.size(); j++)
				{
					node.matrix[j] = MATRIX.at(i);
				}
			}
			node.mesh = NODE.value("mesh", -1);
			node.name = NODE.value("name", "");
			if (NODE.contains("rotation"))
			{
				json ROTATION = NODE["rotation"];
				for (int j = 0; j < ROTATION.size(); j++)
				{
					node.rotation[j] = ROTATION.at(j);
				}
			}
			if (NODE.contains("scale"))
			{
				json SCALE = NODE["scale"];
				for (int j = 0; j < SCALE.size(); j++)
				{
					node.scale[j] = SCALE.at(j);
				}
			}
			node.skin = NODE.value("skin", -1);
			if (NODE.contains("translation"))
			{
				json TRANSLATION = NODE["translation"];
				for (int j = 0; j < TRANSLATION.size(); j++)
				{
					node.translation[j] = TRANSLATION.at(j);
				}
			}
			gltf.nodes.push_back(node);
		}
	}
	if (GLTF.contains("animations"))
	{
		json ANIMATIONS = GLTF["animations"];
		for (int i = 0; i < ANIMATIONS.size(); i++)
		{
			json ANIMATION = ANIMATIONS.at(i);
			json CHANNELS = ANIMATION["channels"];
			vector<AnimationChannel> channels;
			for (int j = 0; j < CHANNELS.size(); j++)
			{
				json CHANNEL = CHANNELS.at(j);
				json TARGET = CHANNEL["target"];
				string path = TARGET["path"];
				AnimationChannelTarget target(path);
				target.node = TARGET.value("node", -1);
				int sampler = CHANNEL["sampler"];
				AnimationChannel channel(sampler, target);
				channels.push_back(channel);
			}
			json SAMPLERS = ANIMATION["samplers"];
			vector<AnimationSampler> samplers;
			for (int j = 0; j < SAMPLERS.size(); j++)
			{
				json SAMPLER = SAMPLERS.at(j);
				int input = SAMPLER["input"];
				int output = SAMPLER["output"];
				AnimationSampler sampler(input, output);
				if (SAMPLER.contains("interpolation"))
				{
					sampler.interpolation = SAMPLER["interpolation"];
				}
				samplers.push_back(sampler);
			}
			Animation animation(channels, samplers);
			animation.name = ANIMATION.value("name", "");
			gltf.animations.push_back(animation);
		}
	}
	if (GLTF.contains("materials"))
	{
		json MATERIALS = GLTF["materials"];
		for (int i = 0; i < MATERIALS.size(); i++)
		{
			json MATERIAL = MATERIALS.at(i);
			string name = MATERIAL.value("name", "");
			Material material;
			if (MATERIAL.contains("alphaCutoff"))
			{
				material.alphaCutoff = MATERIAL["alphaCutoff"];
			}
			if (MATERIAL.contains("alphaMode"))
			{
				material.alphaMode = MATERIAL["alphaMode"];
			}
			if (MATERIAL.contains("doubleSided"))
			{
				material.doubleSided = MATERIAL["doubleSided"];
			}
			if (MATERIAL.contains("emissiveFactor"))
			{
				json EMISSIVE_FACTOR = MATERIAL["emissiveFactor"];
				for (int j = 0; j < EMISSIVE_FACTOR.size(); j++)
				{
					material.emissiveFactor[j] = EMISSIVE_FACTOR.at(j);
				}
			}
			if (MATERIAL.contains("emissiveTexture"))
			{
				json EMISSIVE_TEXTURE = MATERIAL["emissiveTexture"];
				int index = EMISSIVE_TEXTURE["index"];
				material.emissiveTexture = TextureInfo(index);
				if (EMISSIVE_TEXTURE.contains("texCoord"))
				{
					int texCoord = EMISSIVE_TEXTURE["texCoord"];
					material.emissiveTexture.texCoord = texCoord;
				}
			}
			material.name = MATERIAL.value("name", "");
			if (MATERIAL.contains("normalTexture"))
			{
				json NORMAL_TEXTURE = MATERIAL["normalTexture"];
				int index = NORMAL_TEXTURE["index"];
				MaterialNormalTextureInfo normalTexture = MaterialNormalTextureInfo(index);
				material.normalTexture = normalTexture;
				if (NORMAL_TEXTURE.contains("texCoord"))
				{
					int texCoord = NORMAL_TEXTURE["texCoord"];
					material.normalTexture.texCoord = texCoord;
				}
				if (NORMAL_TEXTURE.contains("scale"))
				{
					float scale = NORMAL_TEXTURE["scale"];
					material.normalTexture.scale = scale;
				}
			}
			if (MATERIAL.contains("occlusionTexture"))
			{
				json OCCLUSION_TEXTURE = MATERIAL["occlusionTexture"];
				int index = OCCLUSION_TEXTURE["index"];
				material.occlusionTexture = MaterialOcclusionTextureInfo(index);
				if (OCCLUSION_TEXTURE.contains("strength"))
				{
					material.occlusionTexture.strength = OCCLUSION_TEXTURE["strength"];
				}
				if (OCCLUSION_TEXTURE.contains("texCoord"))
				{
					material.occlusionTexture.texCoord = OCCLUSION_TEXTURE["texCoord"];
				}
			}
			if (MATERIAL.contains("pbrMetallicRoughness"))
			{
				json PBR_METALLIC_ROUGHNESS = MATERIAL["pbrMetallicRoughness"];
				MaterialPBRMetallicRoughness pbrMetallicRoughness;
				if (PBR_METALLIC_ROUGHNESS.contains("baseColorFactor"))
				{
					json BASE_COLOR_FACTOR = PBR_METALLIC_ROUGHNESS["baseColorFactor"];
					for (int j = 0; j < BASE_COLOR_FACTOR.size(); j++)
					{
						pbrMetallicRoughness.baseColorFactor[j] = BASE_COLOR_FACTOR.at(j);
					}
				}
				if (PBR_METALLIC_ROUGHNESS.contains("baseColorTexture"))
				{
					json BASE_COLOR_TEXTURE = PBR_METALLIC_ROUGHNESS["baseColorTexture"];
					int index = BASE_COLOR_TEXTURE["index"];
					TextureInfo baseColorTexture = TextureInfo(index);
					if (BASE_COLOR_TEXTURE.contains("texCoord"))
					{
						baseColorTexture.texCoord = BASE_COLOR_TEXTURE["texCoord"];
					}
					pbrMetallicRoughness.baseColorTexture = baseColorTexture;
				}
				if (PBR_METALLIC_ROUGHNESS.contains("metallicFactor"))
				{
					pbrMetallicRoughness.metallicFactor = PBR_METALLIC_ROUGHNESS["metallicFactor"];
				}
				if (PBR_METALLIC_ROUGHNESS.contains("metallicRoughnessTexture"))
				{
					json METALLIC_ROUGHNESS_TEXTURE = PBR_METALLIC_ROUGHNESS["metallicRoughnessTexture"];
					int index = METALLIC_ROUGHNESS_TEXTURE["index"];
					TextureInfo metallicRoughnessTexture = TextureInfo(index);
					if (METALLIC_ROUGHNESS_TEXTURE.contains("texCoord"))
					{
						metallicRoughnessTexture.texCoord = METALLIC_ROUGHNESS_TEXTURE["texCoord"];
					}
					pbrMetallicRoughness.metallicRoughnessTexture = metallicRoughnessTexture;
				}
				if (PBR_METALLIC_ROUGHNESS.contains("roughnessFactor"))
				{
					pbrMetallicRoughness.roughnessFactor = PBR_METALLIC_ROUGHNESS["roughnessFactor"];
				}
				material.pbrMetallicRoughness = pbrMetallicRoughness;
			}
			gltf.materials.push_back(material);
		}
	}
	if (GLTF.contains("meshes"))
	{
		json MESHES = GLTF["meshes"];
		for (int i = 0; i < MESHES.size(); i++)
		{
			json MESH = MESHES.at(i);
			json PRIMITIVES = MESH["primitives"];
			vector<MeshPrimitive> primitives;
			for (int j = 0; j < PRIMITIVES.size(); j++)
			{
				json PRIMITIVE = PRIMITIVES.at(j);
				json ATTRIBUTES = PRIMITIVE["attributes"];
				vector<pair<string, int>> attributes;
				for (json::iterator it = ATTRIBUTES.begin(); it != ATTRIBUTES.end(); ++it) 
				{
  					attributes.push_back(std::make_pair(it.key(), it.value()));
				}
				MeshPrimitive primitive(attributes);
				primitive.indices = PRIMITIVE.value("indices", -1);
				primitive.material = PRIMITIVE.value("material", -1);
				if (PRIMITIVE.contains("mode"))
				{
					primitive.mode = PRIMITIVE["mode"];
				}
				primitives.push_back(primitive);
			}
			Mesh mesh(primitives);
			mesh.name = MESH.value("name", "");
			if (MESH.contains("weights"))
			{
				json WEIGHTS = MESH["weights"];
				for (int j = 0; j < WEIGHTS.size(); j++)
				{
					int weight = WEIGHTS.at(j);
					mesh.weights.push_back(weight);
				}
			}
			gltf.meshes.push_back(mesh);
		}
	}
	if (GLTF.contains("textures"))
	{
		json TEXTURES = GLTF["textures"];
		for (int i = 0; i < TEXTURES.size(); i++)
		{
			json TEXTURE = TEXTURES.at(i);
			Texture texture;
			texture.name = TEXTURE.value("name", "");
			texture.sampler = TEXTURE.value("sampler", -1);
			texture.source = TEXTURE.value("source", -1);
			gltf.textures.push_back(texture);
		}
	}
	if (GLTF.contains("images"))
	{
		json IMAGES = GLTF["images"];
		for (int i = 0; i < IMAGES.size(); i++)
		{
			json IMAGE = IMAGES.at(i);
			Image image;
			image.bufferView = IMAGE.value("bufferView", -1);
			image.mimeType = IMAGE.value("mimeType", "");
			image.name = IMAGE.value("name", "");
			image.uri = IMAGE.value("uri", "");
			gltf.images.push_back(image);
		}
	}
	if (GLTF.contains("skins"))
	{
		json SKINS = GLTF["skins"];
		for (int i = 0; i < SKINS.size(); i++)
		{
			json SKIN = SKINS.at(i);
			json JOINTS = SKIN["joints"];
			vector<int> joints;
			for (int j = 0; j < JOINTS.size(); j++)
			{
				int joint = JOINTS.at(j);
				joints.push_back(joint);
			}
			Skin skin(joints);
			skin.inverseBindMatrices = SKIN.value("inverseBindMatrices", -1);
			skin.name = SKIN.value("name", "");
			skin.skeleton = SKIN.value("skeleton", -1);
			gltf.skins.push_back(skin);
		}
		
	}
	if (GLTF.contains("accessors"))
	{
		json ACCESSORS = GLTF["accessors"];
		for (int i = 0; i < ACCESSORS.size(); i++)
		{
			json ACCESSOR = ACCESSORS.at(i);
			int componentType = ACCESSOR["componentType"];
			int count = ACCESSOR["count"];
			string type = ACCESSOR["type"];
			Accessor accessor(componentType, count, type);
			accessor.bufferView = ACCESSOR.value("bufferView", -1);
			if (ACCESSOR.contains("byteOffset"))
			{
				accessor.byteOffset = ACCESSOR["byteOffset"];
			}
			if (ACCESSOR.contains("normalized"))
			{
				accessor.normalized = ACCESSOR["normalized"];
			}
			accessor.count = ACCESSOR.value("count", -1);
			if (ACCESSOR.contains("min"))
			{
				json MIN = ACCESSOR["min"];
				for (int j = 0; j < MIN.size(); j++)
				{
					accessor.min.push_back(MIN.at(j));
				}
			}
			if (ACCESSOR.contains("max"))
			{
				json MAX = ACCESSOR["max"];
				for (int j = 0; j < MAX.size(); j++)
				{
					accessor.max.push_back(MAX.at(j));
				}
			}
			gltf.accessors.push_back(accessor);
		}
	}
	if (GLTF.contains("bufferViews"))
	{
		json BUFFER_VIEWS = GLTF["bufferViews"];
		for (int i = 0; i < BUFFER_VIEWS.size(); i++)
		{
			json BUFFER_VIEW = BUFFER_VIEWS.at(i);
			int buffer = BUFFER_VIEW["buffer"];
			int byteLength = BUFFER_VIEW["byteLength"];
			BufferView bufferView(buffer, byteLength);
			bufferView.byteOffset = BUFFER_VIEW.value("byteOffset", -1);
			bufferView.byteStride = BUFFER_VIEW.value("byteStride", -1);
			bufferView.name = BUFFER_VIEW.value("name", "");
			bufferView.target = BUFFER_VIEW.value("target", -1);
			gltf.bufferViews.push_back(bufferView);
		}
	}
	if (GLTF.contains("samplers"))
	{
		json SAMPLERS = GLTF["samplers"];
		for (int i = 0; i < SAMPLERS.size(); i++)
		{
			json SAMPLER = SAMPLERS.at(i);
			Sampler sampler;
			sampler.magFilter = SAMPLER.value("magFilter", -1);
			sampler.minFilter = SAMPLER.value("minFilter", -1);
			sampler.name = SAMPLER.value("name", "");
			if (SAMPLER.contains("wrapS"))
			{
				sampler.wrapS = SAMPLER["wrapS"];
			}
			if (SAMPLER.contains("wrapT"))
			{
				sampler.wrapT = SAMPLER["wrapT"];
			}
			gltf.samplers.push_back(sampler);
		}
		
	}
	if (GLTF.contains("buffers"))
	{
		json BUFFERS = GLTF["buffers"];
		for (int i = 0; i < BUFFERS.size(); i++)
		{
			json BUFFER = BUFFERS.at(i);
			int byteLength = BUFFER["byteLength"];
			Buffer buffer(byteLength);
			buffer.name = BUFFER.value("name", "");
			buffer.uri = BUFFER.value("uri", "");
			gltf.buffers.push_back(buffer);
		}
	}
	return gltf;
}
}
#endif