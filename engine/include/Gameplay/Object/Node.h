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
	virtual void Tick(float deltaTime) override
	{
		Object::Tick(deltaTime);
		for (int i = 0; i < children.size(); i++)
		{
			children[i]->Tick(deltaTime);
		}
	}
};

template<typename T>
void InvForEachNode(T* node, function<void(T*)> func)
{
	for (int i = 0; i < ((Node*)node)->children.size(); i++)
	{
		InvForEachNode((T*)(((Node*)node)->children[i]), func);
	}
	func(node);
}
template<typename T>
void ForEachNode(T* node, function<bool(T*)> func)
{
	if(!func(node))
		return;
	for (int i = 0; i < ((Node*)node)->children.size(); i++)
	{
		ForEachNode((T*)(((Node*)node)->children[i]), func);
	}
}
template<typename T>
T* FindNodeByName(const string& name, T* node)
{
	T* found = nullptr;
	ForEachNode<T>(node, [&found, &name](T* current){
		if (current->name == name)
		{
			found = current;
			return false;
		}
		return true;
	});
	return found;
}
#endif