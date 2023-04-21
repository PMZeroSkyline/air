#ifndef ACTOR_H
#define ACTOR_H

#include "Entity.h"
#include "Gameplay/Component/AnimationNodeComponent.h"

class Actor : public Entity
{
public:
	bool isDirty = true;
	mat4 worldMatrix;
	Transform localTransform;
	void ResetWorldMatrix(bool isForce = false)
	{
		AnimationNodeComponent* animationNodeComponent = GetComponent<AnimationNodeComponent>();
		bool isReset = isForce || isDirty || animationNodeComponent;
		if (isReset)
		{
			mat4 world = parent ? ((Actor*)parent)->worldMatrix : mat4();
			if (animationNodeComponent)
			{
				worldMatrix = world * animationNodeComponent->GetAnimationTransform().ToMatrix();
			}
			else
			{
				worldMatrix = world * localTransform.ToMatrix();
			}
			isDirty = false;
		}
		for (int i = 0; i < children.size(); i++)
		{
			((Actor*)children[i])->ResetWorldMatrix(isReset);
		}
	}
};
#endif