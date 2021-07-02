#include "gondola.h"

#include <cmath>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <world_state.h>

Gondola::Gondola() :
    AeroObject(0.1, 50.0)
{
    mass = 50.0;
    inertia = 50.0;
    width = 40.0;
    height = 30.0;
    bitmap = nullptr;
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
    if (bitmap == nullptr)
    {
        // Save the old bitmap target
        ALLEGRO_BITMAP* const prev_bitmap = al_get_target_bitmap();

        // Create the new bitmap
        bitmap = al_create_bitmap(width, height);
        al_set_target_bitmap(bitmap);

        // Draw the basic rectangle
        al_draw_filled_rectangle(
            0.0f,
            0.0f,
            static_cast<float>(width),
            static_cast<float>(height),
            al_map_rgb(100, 100, 100));
        al_draw_rectangle(
            0.0f,
            0.0f,
            static_cast<float>(width),
            static_cast<float>(height),
            al_map_rgb(0, 0, 0),
            4.0f);

        // Reset the bitmap target
        al_set_target_bitmap(prev_bitmap);
    }

    // Draw the resulting bitmap on screen
    al_draw_rotated_bitmap(
        bitmap,
        static_cast<float>(width) / 2.0f,
        static_cast<float>(height) / 2.0f,
        static_cast<float>(position.x - state->draw_offset.x),
        static_cast<float>(position.y - state->draw_offset.y),
        static_cast<float>(rotation),
        0);
}

void Gondola::pre_step(const StepState* state)
{
    // Run the super-pre-step
    AeroObject::pre_step(state);

    // Check the correct step state
    const WorldState* world_state = dynamic_cast<const WorldState*>(state);
    if (world_state == nullptr)
    {
        throw std::runtime_error("world step must get a physics object for correct computations");
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
        const Vector2 surf_norm = Vector2(0.0, -1.0);

        // Setup the normal and frictional forces
        Vector2 normal_force;
        Vector2 frictional_force;

        double downward_force = 0.0;

        // Calculate the terrain forces if necessary
        if (it->y > elev)
        {
            // Calculate the normal force spring and damping forces
            Vector2 spring_force = it->distance_to(Vector2(it->x, elev)) * world_state->terrain->get_spring_constant() * surf_norm;

            const double upward_damping_force = -surf_norm.dot(point_vel);
            //const double downward_damping_force = -0.1 * mass * world_state->gravity.magnitude();
            const double downward_damping_force = 0.0;

            Vector2 damping_force = std::max(downward_damping_force, upward_damping_force) * world_state->terrain->get_damping_coefficient() * surf_norm;
            //damping_force = damping_force * std::min((damping_force.magnitude_squared() / spring_force.magnitude_squared()), 0.1);

            normal_force += spring_force;
            normal_force += damping_force;

            // Determine the along-terrain force and magnitude velocity-wise
            Vector2 along_terrain = surf_norm.rotate_deg(90.0);
            along_terrain *= -velocity.dot(along_terrain);

            // Determine static vs. kinematic frictional coefficient
            const double FRICTION_COEFF = (std::abs(point_vel.dot(along_terrain)) > 1.0) ? 0.15 : 0.25;

            // Determine the downward force on the object
            downward_force = std::max(0.0, normal_force.dot(surf_norm));

            // Set the frictional force
            frictional_force = FRICTION_COEFF * downward_force * along_terrain;
        }

        // Apply the forces
        add_force_absolute(normal_force, point_offset);
        add_force_absolute(frictional_force, point_offset);
    }
}

void Gondola::step(const StepState* state)
{
    // Limit the moments allowed
    const double MOMENT_LIM = 2000.0;
    moments = std::max(-MOMENT_LIM, std::min(MOMENT_LIM, moments));

    // Call the super-state
    AeroObject::step(state);
}

Gondola::~Gondola()
{
    if (bitmap != nullptr)
    {
        al_destroy_bitmap(bitmap);
        bitmap = nullptr;
    }
}
