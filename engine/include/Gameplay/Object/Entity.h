#ifndef ENTITY_H
#define ENTITY_H

#include "Node.h"
#include "Gameplay/Component/Component.h"

class Entity : public Node
{
public:
	vector<Component*> components;
	bool isTick = true;
	~Entity()
	{
		for (int i = 0; i < components.size(); i++)
		{
			delete components[i];
			components[i] = nullptr;
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
	template<typename T>
	T* AddComponent(T* c)
	{
		((Component*)c)->owner = this;
		components.push_back(c);
		return c;
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
	template<typename T>
	void GetComponents(vector<T*>& result)
	{
		for (int i = 0; i < components.size(); i++)
		{
			Component* c = components[i];
			if (typeid(*c) == typeid(T))
			{
				result.push_back((T*)components[i]);
			}
		}
	}
	template<typename T>
	vector<T*> GetComponents(bool isRecursive = false)
	{
		vector<T*> result;
		if (isRecursive)
		{
			RForEachNode<Entity>([&result](Entity* curr){
				curr->GetComponents<T>(result);
			});
		}
		else
		{
			GetComponents<T>(result);
		}
		return result;
	}
	virtual void Start() override
	{
		Node::Start();
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i])
			{
				components[i]->Start();
			}
		}
	}
	virtual void Tick() override
	{
		if (!isTick)
		{
			return;
		}
		Node::Tick();
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i])
			{
				components[i]->Tick();
			}
		}
	}
	
};

#endif