#include "weight.h"

#include <stdexcept>

#include <allegro5/allegro_primitives.h>

#include <world_state.h>

Weight::Weight() :
    AeroObject(0.1, 0.1)
{
    radius = 10.0;
    mass = 25.0;
    inertia = 1.0;
}

void Weight::draw(const DrawState* state)
{
    const Vector2 screen_position = position - state->draw_offset;

    al_draw_filled_circle(
        static_cast<float>(screen_position.x),
        static_cast<float>(screen_position.y),
        static_cast<float>(radius),
        al_map_rgb(123, 79, 44));
}

void Weight::pre_step(const StepState* state)
{
    // Run any super items
    AeroObject::pre_step(state);

    const WorldState* world_state = dynamic_cast<const WorldState*>(state);
    if (world_state == nullptr)
    {
        throw std::runtime_error("world step must get a physics object for correct computations");
    }

    // Determine the elevation and surface normal of the terrain
    const Vector2 norm = world_state->terrain->surface_normal_at_x(position.x);

    // Determine the point that we are using for elevation
    const Vector2 elev_point = Vector2(
        position.x,
        world_state->terrain->elevation_at_x(position.x));

    // Determine the touch point that we will use for the weight object
    const Vector2 touch_point = position - norm * radius;

    // Determine the distance to the ground (or that we are below the ground)
    const double ground_dist = (touch_point - elev_point).dot(norm);

    // Setup the normal force values
    Vector2 normal_force = Vector2(0.0, 0.0);

    // If below the ground, apply the normal force
    if (ground_dist < 0.0)
    {
        normal_force += -ground_dist * world_state->terrain->get_spring_constant() * norm;
        normal_force += -std::min(0.0, norm.dot(velocity)) * world_state->terrain->get_damping_coefficient() * norm;
    }

    // Add the force to the weight
    add_force_absolute(normal_force);
}
