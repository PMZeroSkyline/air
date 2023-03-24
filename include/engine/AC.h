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
class Actor;
class Component : public Object
{
public:
	Actor* owner;
};
class Actor : public Object
{
public:
	mat4 modelMatrix;
	Transform localTransform;
	map<string, unique_ptr<Component>> componentMap;
	Actor* parent;
	list<unique_ptr<Actor>> childrenList;
	template<typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		map<string, unique_ptr<Component>>::iterator found = componentMap.find(typeid(T).name());
		if (found != componentMap.end() && found->second)
		{
			return (T*)found->second.get();
		}
		else
		{
			unique_ptr<T> added = make_unique<T>(std::forward<Args>(args)...);
			T* result = added.get();
			((Component*)result)->owner = this;
			componentMap[typeid(T).name()] = move(added);
			return result;
		}
	}
	template<typename T, typename... Args>
	T* AddChild(Args&&... args)
	{
		childrenList.push_back(make_unique<T>(std::forward<Args>(args)...));
		childrenList.back()->parent = this;
		return childrenList.back().get();
	}
	template<typename T>
	T* GetComponent()
	{
		map<string, unique_ptr<Component>>::iterator found = componentMap.find(typeid(T).name());
		if (found != componentMap.end() && found->second)
		{
			return (T*)found->second.get();
		}
		else
		{
			return nullptr;
		}
	}
};


#endif