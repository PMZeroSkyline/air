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

void ResetWorldMatrix(Actor* node)
{
	ForEachNode(node, [](Node* current){
		Actor* actor = (Actor*)current;
		AnimationNodeComponent* animationNodeComponent = actor->GetComponent<AnimationNodeComponent>();
		if (actor->parent)
		{
			mat4 world = ((Actor*)actor->parent)->worldMatrix;
			if (animationNodeComponent)
			{
				actor->worldMatrix = world * animationNodeComponent->GetAnimationTransform().ToMatrix();
			}
			else
			{
				actor->worldMatrix = world * actor->localTransform.ToMatrix();
			}
		}
		else
		{
			if (animationNodeComponent)
			{
				actor->worldMatrix = animationNodeComponent->GetAnimationTransform().ToMatrix();
			}
			else
			{
				actor->worldMatrix = actor->localTransform.ToMatrix();
			}
		}
		return true;
	});
}

#endif