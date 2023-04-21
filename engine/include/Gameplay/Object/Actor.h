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
	void ResetWorldMatrix(bool isForceReset = false)
	{
		AnimationNodeComponent* animationNodeComponent = GetComponent<AnimationNodeComponent>();
		bool isReset = isForceReset || isDirty || animationNodeComponent;
		if (isReset)
		{
			if (parent)
			{
				mat4 world = ((Actor*)parent)->worldMatrix;
				if (animationNodeComponent)
				{
					worldMatrix = world * animationNodeComponent->GetAnimationTransform().ToMatrix();
				}
				else
				{
					worldMatrix = world * localTransform.ToMatrix();
				}
			}
			else
			{
				if (animationNodeComponent)
				{
					worldMatrix = animationNodeComponent->GetAnimationTransform().ToMatrix();
				}
				else
				{
					worldMatrix = localTransform.ToMatrix();
				}
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