#ifndef NODE_H
#define NODE_H

#include "Core/Transform/Transform.h"
#include "Core/Math/Math.h"
#include "Gameplay/Object/Object.h"
#include "Gameplay/Component/Component.h"


class Node : public Object
{
public:
	Node* parent;
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
};


#endif