#ifndef MAP_H
#define MAP_H

#include "Gameplay/Object/Actor.h"

class Map : public Actor
{
public:
    Map()
    {
    }
    ~Map()
    {
    }
    virtual void Start() override
    {
        Actor::Start();

    }
    virtual void Tick() override
    {
        Actor::Tick();
    }
};

#endif