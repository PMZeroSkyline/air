#ifndef COMPONENT_H
#define COMPONENT_H

#include "Gameplay/Object/Object.h"

class Entity;
class Component : public Object
{
public:
	Entity* owner;

};

#endif