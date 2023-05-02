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
	Window* window = GetCurrentWindowContext();
	
	vec3 GetForwardVector() const
	{
		const vec4 c = worldMatrix.column(0);
		return vec3(c.x, c.y, c.z);
	}
	vec3 GetUpVector() const
	{
		const vec4 c = worldMatrix.column(2);
		return vec3(c.x, c.y, c.z);
	}
	vec3 GetRightVector() const
	{
		const vec4 c = worldMatrix.column(1);
		return vec3(c.x, c.y, c.z);
	}
	void SetWorldMatrix(mat4 m)
	{
		mat4 IW = parent ? ((Actor*)parent)->worldMatrix.inverse() : mat4();
		mat4 localMatrix = IW * m;
		localTransform = Transform(localMatrix);
		ResetWorldMatrix(true);
	}
	void AddWorldTranslation(const vec3& translation)
	{
		for (int i = 0; i < 3; i++)
		{
			worldMatrix[i][3] += translation[i];
		}
		SetWorldMatrix(worldMatrix);
	}
	void SetWorldRotation(const quat& rotation)
	{
		mat4 rotationMatrix(rotation);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				worldMatrix[i][j] = rotationMatrix[i][j];
			}
		}
		SetWorldMatrix(worldMatrix);
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
	void ResetRoot(Actor* target)
	{
		AnimationNodeComponent* animationNodeComponent = GetComponent<AnimationNodeComponent>();
		bool hasAnimation = false;
		Transform animationTransform;
		if (animationNodeComponent)
		{
			animationTransform = animationNodeComponent->GetAnimationTransform();
			hasAnimation = animationTransform != Transform();
		}
		mat4 world = parent ? ((Actor*)parent)->worldMatrix : mat4();
		if (name == "Root")
		{
			Transform preAnimationTransform = animationNodeComponent->GetAnimationTransform(-window->deltaTime);
			mat4 animationMatrix = world * animationTransform.ToMatrix();
			mat4 preAnimationMatrix = world * preAnimationTransform.ToMatrix();
			mat4 rootMotionMatrix = animationMatrix / preAnimationMatrix;
			target->SetWorldMatrix(rootMotionMatrix * target->worldMatrix);
			worldMatrix = world * localTransform.ToMatrix();
		}
		else
		{
			mat4 local = hasAnimation ? animationTransform.ToMatrix() : localTransform.ToMatrix();
			worldMatrix = world * local;
		}
		isDirty = false;
		for (int i = 0; i < children.size(); i++)
		{
			((Actor*)children[i])->ResetRoot(target);
		}
	}
};
#endif