#ifndef MODEL_H
#define MODEL_H

#include "math.h"
#include <vector>
using std::vector;

struct mesh
{
	vector<vec3> p;
	vector<vec3> n;
	vector<vec4> t;
	vector<vec2> uv;
	vector<unsigned int> ids;
};
mesh make_quad()
{
	mesh m;
	m.p = {{-1, 1, 0}, {1, 1, 0}, {1, -1, 0}, {-1, -1, 0}};
	m.uv = {{0, 1}, {1, 1}, {1, 0}, {0, 0}};
	m.ids = {0, 2, 1, 0, 3, 2};
	return m;
}

#endif