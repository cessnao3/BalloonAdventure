#ifndef GIO_PHYSICS_OBJECT_H
#define GIO_PHYSICS_OBJECT_H

#include "game_object.h"
//import Vector;import vector2;

struct PhysicsState : public StepState
{
    Vector2 gravity;
};

class PhysicsObject : public GameObject
{
public:
    PhysicsObject() :
        position(),
        velocity(),
        rotation(0.0),
        rotational_vel(0.0),
        mass(1.0),
        inertia(1.0),
        forces(),
        moments(0.0)
    {
        // Do Nothing
    }

    void add_force_relative(const Vector2& force, const Vector2& offset)
    {
        add_force_absolute(
            force.rotate_rad(rotation),
            offset);
    }

    void add_force_absolute(const Vector2& force, const Vector2& offset)
    {
        forces += force;
        moments += offset.cross(force);
    }

    virtual void step(const StepState* state) override
    {
        // Extract the physics state
        const PhysicsState* physics = dynamic_cast<const PhysicsState*>(state);
        if (physics == nullptr)
        {
            return;
        }

        // Extract the time step
        const double dt = physics->time_step;

        // Integrate translational motion
        velocity += forces / mass * dt + physics->gravity * dt;
        position += velocity * dt;

        // Integrate rotational motion
        rotational_vel += moments / inertia * dt;
        rotation = rotational_vel * dt;

        // Clear the forces and moments
        forces = Vector2(0.0, 0.0);
        moments = 0.0;
    }

protected:
    Vector2 position;
    Vector2 velocity;
    
    double rotation;
    double rotational_vel;

    double mass;
    double inertia;

    Vector2 forces;
    double moments;
};

#endif // GIO_PHYSICS_OBJECT_H
