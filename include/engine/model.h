#ifndef MODEL_H
#define MODEL_H

#include "node.h"
#include "res.h"
#include "image.h"
#include "mesh.h"
#include "math.h"
#include "gltf.h"
#include "gl.h"
#include "window.h"


struct c_mesh : comp
{
	string name;
};
struct model : node
{
	vector<node*> nodes;
	vector<c_mesh*> c_meshs;

	void load(const string &path)
	{
		string root_dir = path.substr(0, path.find_last_of('/')+1);

		gltf::glTF gltf = gltf::load(path);

		gltf::Scene g_scene = gltf.scenes[gltf.scene];
		this->name = g_scene.name;

		nodes.resize(gltf.nodes.size());
		c_meshs.resize(gltf.meshes.size());

		load_tree(this, gltf, g_scene.nodes);
	}
	void load_tree(node* p, const gltf::glTF &gltf, const vector<int> csId)
	{
		for (int i = 0; i < csId.size(); i++)
		{
			gltf::Node g_node = gltf.nodes[csId[i]];

			node *c = p->add_child<node>();

			nodes[csId[i]] = c;

			c->name = g_node.name;

			mat4 mtx = g_node.matrix;
			vec3 tra = g_node.translation;
			quat rot = g_node.rotation;
			vec3 sca = g_node.scale;
			if (mtx != mat4())
			{
				c->local = transform(mtx);
			}
			else if(tra != vec3(0) || rot != quat() || sca != vec3(1))
			{
				c->local.p = tra;
				c->local.r = rot;
				c->local.s = sca;
			}

			if (g_node.mesh != -1)
			{
				c_mesh *cm = c->add_comp<c_mesh>();
				c_meshs[g_node.mesh] = cm;
				gltf::Mesh g_mesh = gltf.meshes[g_node.mesh];

				cm->name = g_mesh.name;
			}
			

			load_tree(c, gltf, g_node.children);
		}
	}
};


#endif