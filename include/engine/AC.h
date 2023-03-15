#ifndef AC_H
#define AC_H

#include "Core.h"

class Object
{
public:
	string name;
	virtual ~Object()
	{
		LOG("delete " + name)
	};
	virtual void Start(){};
	virtual void Tick(){};
};
class Component : public Object
{
public:
	weak_ptr<Object> owner;
};
class Actor : public Object
{
public:
	map<string, shared_ptr<Component>> componentMap;
	shared_ptr<Actor> parent;
	list<shared_ptr<Actor>> childrenList;
};


#endif