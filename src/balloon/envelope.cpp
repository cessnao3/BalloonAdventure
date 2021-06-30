#include "envelope.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

Envelope::Envelope() :
    AeroObject(0.5),
    bitmap(nullptr)
{
    // Define the radius
    radius = 60.0;

    // Define phsyics parameters
    mass = 10.0;
    inertia = 5.0;
}

double Envelope::get_radius() const
{
    return radius;
}

Vector2 Envelope::anchor_point_left() const
{
    return position - Vector2(radius, 0.0).rotate_deg(-30).rotate_rad(rotation);
}

Vector2 Envelope::anchor_point_right() const
{
    return position + Vector2(radius, 0.0).rotate_deg(30).rotate_rad(rotation);
}

void Envelope::draw(const DrawState* state)
{
    // Draw the envelope
    al_draw_filled_circle(
        static_cast<float>(position.x),
        static_cast<float>(position.y),
        radius,
        al_map_rgb(255, 0, 0));

    // Define the anchor points
    const Vector2 a_left = anchor_point_left();
    const Vector2 a_right = anchor_point_right();

    // Draw the anchor points
    al_draw_filled_circle(
        static_cast<float>(a_left.x),
        static_cast<float>(a_left.y),
        5.0f,
        al_map_rgb(0, 0, 0));
    al_draw_filled_circle(
        static_cast<float>(a_right.x),
        static_cast<float>(a_right.y),
        5.0f,
        al_map_rgb(0, 0, 0));
}

void Envelope::pre_step(const StepState* state)
{
    // Perform Pre-Step Items
    AeroObject::pre_step(state);

    // Setup lift
    double vert_force = 300.0;
    if (state->input_manager->get_dir_up())
    {
        vert_force += 400.0;
    }
    if (state->input_manager->get_dir_down())
    {
        vert_force -= 200.0;
    }

    // Setup lateral forces
    double lat_force = 0.0;
    if (state->input_manager->get_dir_right())
    {
        lat_force += 200.0;
    }
    if (state->input_manager->get_dir_left())
    {
        lat_force += -200.0;
    }

    // Apply forces
    add_force_absolute(Vector2(lat_force, -vert_force));
}

Envelope::~Envelope()
{
    if (bitmap != nullptr)
    {
        al_destroy_bitmap(bitmap);
        bitmap = nullptr;
    }
}
