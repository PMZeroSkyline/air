#ifndef MESH_H
#define MESH_H

#include <vector>
using std::vector;
using std::pair;
#include <memory>
using std::shared_ptr;

#include "math.h"
#include "res.h"
#include "gl.h"
#include "image.h"


struct sampler2D : res
{
	int wrapS;
	int wrapT;
	int minFilter;
	int magFilter;
	bool mipmap;
};
struct texture2D : res
{
	shared_ptr<sampler2D> sampler;
	shared_ptr<image> img;
	shared_ptr<gl_texture2D> gl_tex;
};
struct material : res
{
	shared_ptr<gl_program> gl_prog;
	vector<pair<string, shared_ptr<texture2D>>> texs;
};
struct mesh_attr
{
	vector<vec3> p;
	vector<vec3> n;
	vector<vec4> t;
	vector<vec4> c;
	vector<vec2> uv;
	vector<vec2> uv1;
	vector<ivec4> bid;
	vector<vec4> bw;
	vector<unsigned int> ids;
	vec3 min;
	vec3 max;
};
struct mesh_primitive
{
	mesh_attr attr;
	int ebo_size;
	shared_ptr<gl_verts> gl_vs;
	shared_ptr<material> mat;
};
struct mesh : res
{
	string name;
	vector<mesh_primitive> prims;
};
mesh_attr make_quad()
{
	mesh_attr m;
	m.p = {{-1, 1, 0}, {1, 1, 0}, {1, -1, 0}, {-1, -1, 0}};
	m.uv = {{0, 1}, {1, 1}, {1, 0}, {0, 0}};
	m.ids = {0, 2, 1, 0, 3, 2};
	return m;
}
#endif