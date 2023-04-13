#ifndef ACTOR_H
#define ACTOR_H

#include "Entity.h"

class Actor : public Entity
{
public:
	mat4 worldMatrix;
	Transform localTransform;
	void TickWorldMatrix()
	{
		if (parent)
		{
			worldMatrix = ((Actor*)parent)->worldMatrix * localTransform.ToMatrix();
		}
		else
		{
			worldMatrix = localTransform.ToMatrix();
		}
		for (int i = 0; i < children.size(); i++)
		{
			((Actor*)children[i])->TickWorldMatrix();
		}
	}
};

#endif