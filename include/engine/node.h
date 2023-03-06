#ifndef NODE_H
#define NODE_H
#include <vector>
#include <string>
using std::vector;
using std::string;
#include "math.h"

struct node;
struct comp
{
	node* o = nullptr;
	virtual ~comp();
	virtual void start(){}
	virtual void tick(){}
};
struct node
{
	node* p = nullptr;
	vector<node*> cs;
	vector<comp*> comps;
	string name;
	transform local;
	mat4 world;
	virtual ~node()
	{
		for (int i = 0; i < comps.size(); i++)
		{
			comps[i]->o = nullptr;
			delete comps[i];
			comps[i] = nullptr;
		}
		for (int i = 0; i < cs.size(); i++)
		{
			delete cs[i];
			cs[i] = nullptr;
		}
	}
	virtual void start(){}
	virtual void tick(){}
	template<typename T, typename... Args>
	T* add_comp(Args&&... args)
	{
		T* c = new T(std::forward<Args>(args)...);
		((comp*)c)->o = this;
		comps.push_back(c);
		return c;
	}
	void add_comp(comp *c)
	{
		c->o = this;
		comps.push_back(c);
	}
	template<typename T>
	T* get_comp()
	{
		for (int i = 0; i < comps.size(); i++)
		{
			comp *&c = comps[i];
			if (typeid(T) == typeid(*c))
				return (T*)comps[i];
		}
		return nullptr;
	}
	template<typename T, typename... Args>
	T* add_child(Args&&... args)
	{
		T* c = new T(std::forward<Args>(args)...);
		((node*)c)->p = this;
		cs.push_back(c);
		return c;
	}
	void add_child(node *c)
	{
		c->p = this;
		cs.push_back(c);
	}
	void tick_world_matrix()
	{
		if (p)
		{
			world = p->world * local.to_matrix();
		}
		else
		{
			world = local.to_matrix();
		}
		for (int i = 0; i < cs.size(); i++)
		{
			cs[i]->tick_world_matrix();
		}
	}
};
comp::~comp()
{
	if (o)
		for (int i = 0; i < o->comps.size(); i++)
			if (o->comps[i] == this)
				o->comps.erase(o->comps.begin()+i);
}
void destroy(node* &p)
{
	delete p;
	p = nullptr;
}
void destroy(comp* &p)
{
	delete p;
	p = nullptr;
}

void tree_each(node* &n, void (*node_callback)(node* &n))
{
	if (n)
	{
		node_callback(n);
		for (int i = 0; i < n->cs.size(); i++)
			tree_each(n->cs[i], node_callback);
	}
}
void tree_each(node* &n, void (*node_callback)(node* &n), void (*comp_callback)(comp* &c))
{
	if (n)
	{
		node_callback(n);
		for (int i = 0; i < n->comps.size(); i++)
			comp_callback(n->comps[i]);
		for (int i = 0; i < n->cs.size(); i++)
			tree_each(n->cs[i], node_callback, comp_callback);
	}
}

#endif