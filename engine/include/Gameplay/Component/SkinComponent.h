#ifndef SKIN_COMPONENT_H
#define SKIN_COMPONENT_H

#include "Gameplay/Component/Component.h"
#include "Animation/SkinInstance.h"

class SkinComponent : public Component
{
public:
    SkinInstance* skinInstance = nullptr;
};

#endif