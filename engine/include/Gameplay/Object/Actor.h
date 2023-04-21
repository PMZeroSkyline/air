#ifndef ACTOR_H
#define ACTOR_H

#include "Entity.h"
#include "Gameplay/Component/AnimationNodeComponent.h"

class Actor : public Entity
{
public:
	mat4 worldMatrix;
	Transform localTransform;
};

void ForEachActor(Actor* node, void (*F)(Actor*))
{
	F(node);
	for (int i = 0; i < node->children.size(); i++)
	{
		ForEachActor((Actor*)node->children[i], F);
	}
}

void ResetWorldMatrix(Actor* node)
{
	AnimationNodeComponent* animationNodeComponent = node->GetComponent<AnimationNodeComponent>();
	mat4 parentMatrix;
	if (node->parent)
	{
		parentMatrix = ((Actor*)node->parent)->worldMatrix;
	}
	if (animationNodeComponent)
	{
		node->worldMatrix = parentMatrix * animationNodeComponent->GetAnimationTransform().ToMatrix();
	}
	else
	{
		node->worldMatrix = parentMatrix * node->localTransform.ToMatrix();
	}
	for (int i = 0; i < node->children.size(); i++)
	{
		ResetWorldMatrix((Actor*)node->children[i]);
	}
}

#endif