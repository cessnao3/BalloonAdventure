#include "gondola.h"

#include <cmath>

#include <allegro5/allegro_primitives.h>

#include <world_state.h>

Gondola::Gondola() :
    AeroObject(0.1)
{
    mass = 50.0;
    inertia = 3.0;
    width = 40.0;
    height = 30.0;
}

Vector2 Gondola::get_top_left() const
{
    return position + Vector2(
        -width / 2.0,
        -height / 2.0).rotate_rad(rotation);
}

Vector2 Gondola::get_top_right() const
{
    return position + Vector2(
        width / 2.0,
        -height / 2.0).rotate_rad(rotation);
}

Vector2 Gondola::get_bottom_left() const
{
    return position + Vector2(
        -width / 2.0,
        height / 2.0).rotate_rad(rotation);
}

Vector2 Gondola::get_bottom_right() const
{
    return position + Vector2(
        width / 2.0,
        height / 2.0).rotate_rad(rotation);
}

std::vector<Vector2> Gondola::get_points() const
{
    return {
        get_top_left(),
        get_bottom_left(),
        get_bottom_right(),
        get_top_right()
    };
}

void Gondola::draw(const DrawState* state)
{
    // Define the core points
    const std::vector<Vector2> points = get_points();

    // Add each point to a vector for use in drawing
    std::vector<float> al_points;

    for (auto it = points.begin(); it != points.end(); ++it)
    {
        Vector2 gondola_point = *it - state->draw_offset;
        al_points.push_back(static_cast<float>(gondola_point.x));
        al_points.push_back(static_cast<float>(gondola_point.y));
    }

    // Draw the results
    al_draw_filled_polygon(
        al_points.data(),
        static_cast<int>(points.size()),
        al_map_rgb(100, 100, 100));
    al_draw_polygon(
        al_points.data(),
        static_cast<int>(points.size()),
        ALLEGRO_LINE_JOIN_BEVEL,
        al_map_rgb(0, 0, 0),
        2.0,
        0.5);
}

void Gondola::pre_step(const StepState* state)
{
    // Check the correct step state
    const WorldState* world_state = dynamic_cast<const WorldState*>(state);
    if (world_state == nullptr)
    {
        return;
    }

    // Define the points to check parameters for
    const std::vector<Vector2> points = get_points();

    // Check each corner for hitting the ground
    for (auto it = points.begin(); it != points.end(); ++it)
    {
        // Determine the elevation point for the current point
        const double elev = world_state->terrain->elevation_at_x(it->x);

        // Determine the offset to the center and the resulting point velocity
        const Vector2 point_offset = *it - position;
        const Vector2 point_vel = velocity + Vector2(
            -point_offset.y,
            point_offset.x) * rotational_vel;

        // Obtain the surface normal
        const Vector2 surf_norm = world_state->terrain->surface_normal_at_x(it->x);

        // Setup the normal and frictional forces
        Vector2 normal_force;
        Vector2 frictional_force;

        double downward_force = 0.0;

        // Calculate the normal force if needed
        if (it->y > elev)
        {
            normal_force += it->distance_to(Vector2(it->x, elev)) * world_state->terrain->get_spring_constant() * surf_norm;

            downward_force = -std::min(0.0, normal_force.dot(surf_norm));

            normal_force += -std::min(0.0, surf_norm.dot(point_vel)) * world_state->terrain->get_damping_coefficient() * surf_norm;
        }

        // Calculate the frictional force if needed
        if (it->y > elev - 1)
        {
            // Determine the along-terrain force and magnitude velocity-wise
            Vector2 along_terrain = surf_norm.rotate_deg(90.0);
            along_terrain *= -velocity.dot(along_terrain);

            // Determine static vs. kinematic frictional coefficient
            const double FRICTION_COEFF = (std::abs(point_vel.dot(along_terrain)) > 1.0) ? 0.15 : 0.25;

            // Set the frictional force
            frictional_force = FRICTION_COEFF * downward_force * along_terrain;
        }

        // Apply the forces
        add_force_absolute(normal_force, point_offset);
        add_force_absolute(frictional_force, point_offset);
    }

    // Add rotational damping term
    moments += -std::pow(rotational_vel, 2.0) * 0.1;
}

void Gondola::step(const StepState* state)
{
    // Limit the moments seen
    const double MOMENT_LIM = 2000.0;
    moments = std::min(std::max(-MOMENT_LIM, moments), MOMENT_LIM);

    // Perform the normal step
    AeroObject::step(state);
}
