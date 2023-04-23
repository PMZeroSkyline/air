#ifndef OBJECT_H
#define OBJECT_H

#include "SDK/STL/STL.h"
#include "Core/Log/Log.h"

class Object
{
public:
	string name;
	virtual ~Object(){LOG("delete " << name)}
	virtual void Start(){}
	virtual void Tick(){}
	
};

#endif