#ifndef WORLD_STATE_H
#define WORLD_STATE_H

#include <gamelib/physics_object.h>

#include <terrain.h>

struct WorldState : PhysicsState
{
    Terrain* terrain = nullptr;
};

#endif // WORLD_STATE_H
