#ifndef WORLD_STATE_H
#define WORLD_STATE_H

#include <gamelib/physics_object.h>

#include <terrain.h>

struct WorldState : PhysicsState
{
    Terrain* terrain;
};

#endif // WORLD_STATE_H
