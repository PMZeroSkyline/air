#ifndef SKIN_COMPONENT_H
#define SKIN_COMPONENT_H

#include "Component.h"
#include "Animation/SkinInstance.h"

class SkinComponent : public Component
{
public:
    SkinInstance* skinInstance;
};

#endif