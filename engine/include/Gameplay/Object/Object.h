#ifndef OBJECT_H
#define OBJECT_H

#include "SDK/STL/STL.h"
#include "Core/Log/Log.h"
#include "OS/Window/Window.h"

class Object
{
public:
	string name;
	Window* window = GetCurrentWindowContext();
	virtual ~Object(){}
	virtual void Start(){}
	virtual void Tick(){}
	virtual void Message(const string& msg) {};
};

#endif