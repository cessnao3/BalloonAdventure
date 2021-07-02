#include <gamelib/physics_object.h>

#include <gamelib/constants.h>

#include <cmath>

#include <stdexcept>

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
    // Run the super-step
    StepObject::step(state);

    // Extract the physics state
    const PhysicsState* physics = dynamic_cast<const PhysicsState*>(state);
    if (physics != nullptr)
    {
        // Extract the time step
        const double dt = physics->time_step;

        // Integrate translational motion
        velocity += forces / mass * dt + physics->gravity * dt;
        position += velocity * dt;

        // Integrate rotational motion
        rotational_vel += moments / inertia * dt;
        rotation += rotational_vel * dt;
        rotation = std::fmod(rotation + gio::pi, 2.0 * gio::pi) - gio::pi;
    }
    else
    {
        throw std::runtime_error("physics step must get a physics object for correct computations");
    }
}

void PhysicsObject::post_step(const StepState* state)
{
    // Run the super-step
    StepObject::post_step(state);
    
    // Clear the forces and moments
    reset_forces();
}

void PhysicsObject::reset_forces()
{
    forces = Vector2(0.0, 0.0);
    moments = 0.0;
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

Vector2 PhysicsObject::get_position() const
{
    return position;
}

Vector2 PhysicsObject::get_velocity() const
{
    return velocity;
}

Vector2 PhysicsObject::get_velocity_at_absolute(const Vector2& point)
{
    const Vector2 offset = point - get_position();
    return velocity + rotational_vel * Vector2(
        -offset.y,
        offset.x);
}
