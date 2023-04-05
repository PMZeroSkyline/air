#ifndef AC_H
#define AC_H

#include "Core.h"
#include "Transform.h"

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
	virtual Object* Instance()
	{
		Object* des = new Object;
		*des = *this;
		return des;
	}
};
class Actor;
class Component : public Object
{
public:
	Actor* owner;
	Object* Instance() override
	{
		Component* des = new Component;
		*des = *this;
		return des;
	}
};
class Actor : public Object
{
public:
	mat4 worldMatrix;
	Transform localTransform;
	vector<Component*> components;
	Actor* parent;
	vector<Actor*> children;
	~Actor()
	{
		for (int i = 0; i < components.size(); i++)
		{
			delete components[i];
			components[i] = nullptr;
		}
		for (int i = 0; i < children.size(); i++)
		{
			delete children[i];
			children[i] = nullptr;
		}
	}
	template<typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		T* add = new T(forward<Args>(args)...);
		((Component*)add)->owner = this;
		components.push_back(add);
		return add;
	}
	template<typename T, typename... Args>
	T* AddChild(Args&&... args)
	{
		T* add = new T(forward<Args>(args)...);
		((Actor*)add)->parent = this;
		children.push_back(add);
		return add;
	}
	template<typename T>
	T* GetComponent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			Component* c = components[i];
			if (typeid(*c) == typeid(T))
			{
				return (T*)components[i];
			}
		}
		return nullptr;
	}
	void TickWorldMatrix()
	{
		if (parent)
		{
			worldMatrix = parent->worldMatrix * localTransform.ToMatrix();
		}
		else
		{
			worldMatrix = localTransform.ToMatrix();
		}
		for (int i = 0; i < children.size(); i++)
		{
			children[i]->TickWorldMatrix();
		}
		
	}
	Object* Instance() override
	{
		Actor* des = new Actor();
		*des = *this;
		for (int i = 0; i < components.size(); i++)
		{
			des->components[i] = (Component*)components[i]->Instance();
			*des->components[i] = *components[i];
			des->components[i]->owner = des;
		}
		for (int i = 0; i < children.size(); i++)
		{
			des->children[i] = (Actor*)children[i]->Instance();
			des->parent = this;
		}
		return des;
	}
};
#endif