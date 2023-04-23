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
	vec4 GetForwardVector() const
	{
		return worldMatrix.column(0);
	}
	vec4 GetUpVector() const
	{
		return worldMatrix.column(1);
	}
	vec4 GetRightVector() const
	{
		return worldMatrix.column(2);
	}
	void SetWorldMatrix(mat4 m)
	{
		mat4 IW = parent ? ((Actor*)parent)->worldMatrix.inverse() : mat4();
		mat4 localMatrix = IW * m;
		localTransform = Transform(localMatrix);
		ResetWorldMatrix(true);
	}
	void ResetWorldMatrix(bool isForce = false)
	{
		AnimationNodeComponent* animationNodeComponent = GetComponent<AnimationNodeComponent>();
		bool hasAnimation = false;
		Transform animationTransform;
		if (animationNodeComponent)
		{
			animationTransform = animationNodeComponent->GetAnimationTransform();
			hasAnimation = animationTransform != Transform();
		}
		bool isReset = isForce || isDirty || hasAnimation;
		if (isReset)
		{
			mat4 world = parent ? ((Actor*)parent)->worldMatrix : mat4();
			mat4 local = hasAnimation ? animationTransform.ToMatrix() : localTransform.ToMatrix();
			worldMatrix = world * local;
			isDirty = false;
		}
		for (int i = 0; i < children.size(); i++)
		{
			((Actor*)children[i])->ResetWorldMatrix(isReset);
		}
	}
};
#endif