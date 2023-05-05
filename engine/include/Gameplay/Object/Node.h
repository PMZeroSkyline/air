#ifndef NODE_H
#define NODE_H

#include "Core/Transform/Transform.h"
#include "Core/Math/Math.h"
#include "Gameplay/Object/Object.h"
#include "Gameplay/Component/Component.h"


class Node : public Object
{
public:
	Node* parent = nullptr;
	vector<Node*> children;
	~Node()
	{
		for (int i = 0; i < children.size(); i++)
		{
			delete children[i];
			children[i] = nullptr;
		}
	}
	template<typename T, typename... Args>
	T* AddChild(Args&&... args)
	{
		T* add = new T(forward<Args>(args)...);
		((Node*)add)->parent = this;
		children.push_back(add);
		return add;
	}
	virtual void Start() override
	{
		Object::Start();
		for (int i = 0; i < children.size(); i++)
		{
			children[i]->Start();
		}
	}
	virtual void Tick() override
	{
		Object::Tick();
		for (int i = 0; i < children.size(); i++)
		{
			children[i]->Tick();
		}
	}
	template<typename T>
	void RForEach(T* node, function<void(T*)> func)
	{
		for (int i = 0; i < node->children.size(); i++)
		{
			RForEach((T*)node->children[i], func);
		}
		func(node);
	}
	template<typename T>
	void RForEach(function<void(T*)> func)
	{
		RForEach((T*)this, func);
	}
	template<typename T>
	void ForEach(T* node, function<bool(T*)> func)
	{
		if (func(node))
		{
			return;
		}
		for (int i = 0; i < node->children.size(); i++)
		{
			ForEach((T*)node->children[i], func);
		}
	}
	template<typename T>
	void ForEach(function<bool(T*)> func)
	{
		ForEach((T*)this, func);
	}
	template<typename T>
	T* FindByName(const string& findName)
	{
		T* result = nullptr;
		ForEach<T>([&result, &findName](T* curr){
			if (((Node*)curr)->name == findName)
			{
				result = curr;
				return true;
			}
			return false;
		});
		return result;
	}
	template<typename T>
	T* GetRoot(T* node)
	{
		T* parent = (T*)((Node*)node)->parent;
		if (parent)
		{
			return GetRoot(parent);
		}
		return node;
	}
	template<typename T>
	T* GetRoot()
	{
		return GetRoot((T*)this);
	}
};


#endif