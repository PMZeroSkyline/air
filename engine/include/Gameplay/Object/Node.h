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

void ForEachNode(Node* node, function<bool(Node*)> func)
{
	if(!func(node))
		return;
	for (int i = 0; i < node->children.size(); i++)
	{
		ForEachNode(node->children[i], func);
	}
}
Node* FindNodeByName(const string& name, Node* node)
{
	Node* found = nullptr;
	ForEachNode(node, [&found, &name](Node* current){
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