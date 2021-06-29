#include "physics_object.h"

#include "constants.h"

PhysicsObject::PhysicsObject() :
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

void PhysicsObject::add_force_relative(const Vector2& force)
{
    add_force_relative(
        force,
        Vector2());
}

void PhysicsObject::add_force_relative(
    const Vector2& force,
    const Vector2& offset)
{
    add_force_absolute(
        force.rotate_rad(rotation),
        offset.rotate_rad(-rotation));
}

void PhysicsObject::add_force_absolute(const Vector2& force)
{
    add_force_absolute(
        force,
        Vector2());
}

void PhysicsObject::add_force_absolute(
    const Vector2& force,
    const Vector2& offset)
{
    forces += force;
    moments += offset.cross(force);
}

void PhysicsObject::step(const StepState* state)
{
    // Extract the physics state
    const PhysicsState* physics = dynamic_cast<const PhysicsState*>(state);
    if (physics != nullptr)
    {
        step_physics(physics);
    }
}

void PhysicsObject::step_physics(const PhysicsState* state)
{
    // Extract the time step
    const double dt = state->time_step;

    // Integrate translational motion
    velocity += forces / mass * dt + state->gravity * dt;
    position += velocity * dt;

    // Integrate rotational motion
    rotational_vel += moments / inertia * dt;
    rotation = rotational_vel * dt;

    // Clear the forces and moments
    forces = Vector2(0.0, 0.0);
    moments = 0.0;

    // Update the rotation parameter
    rotation = std::fmod(rotation + gio::pi, 2.0 * gio::pi) - gio::pi;
}

void PhysicsObject::set_position(const Vector2& pos)
{
    position = pos;
}

void PhysicsObject::set_position(const double x, const double y)
{
    position.x = x;
    position.y = y;
}
