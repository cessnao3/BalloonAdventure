#include "envelope.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

Envelope::Envelope() :
    AeroObject(0.5, 100.0),
    burner_on(false),
    valve_open(false)
{
    // Define the radius
    radius = 60.0;

    // Define phsyics parameters
    mass = 10.0;
    inertia = 5.0;

    // Setup the initial temperature
    current_temperature_ratio = 0.5;
}

double Envelope::get_radius() const
{
    return interpolate_value(60.0, 100.0);
}

Vector2 Envelope::anchor_point_left() const
{
    return position - Vector2(get_radius(), 0.0).rotate_deg(-30).rotate_rad(rotation);
}

Vector2 Envelope::anchor_point_right() const
{
    return position + Vector2(get_radius(), 0.0).rotate_deg(30).rotate_rad(rotation);
}

void Envelope::draw(const DrawState* state)
{
    // Define the screen position
    const Vector2 screen_pos = position - state->draw_offset;

    // Draw the envelope
    al_draw_filled_circle(
        static_cast<float>(screen_pos.x),
        static_cast<float>(screen_pos.y),
        get_radius(),
        al_map_rgb(
            static_cast<unsigned char>(interpolate_value(200.0, 200.0)),
            static_cast<unsigned char>(interpolate_value(100.0, 0.0)),
            static_cast<unsigned char>(interpolate_value(100.0, 0.0))));

    // Define the anchor points
    const Vector2 a_left = anchor_point_left() - state->draw_offset;
    const Vector2 a_right = anchor_point_right() - state->draw_offset;

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

double Envelope::interpolate_value(const double min_val, const double max_val) const
{
    return min_val * (1.0 - current_temperature_ratio) + max_val * current_temperature_ratio;
}

void Envelope::pre_step(const StepState* state)
{
    // Perform Pre-Step Items
    AeroObject::pre_step(state);

    // Setup lift
    double vert_force = interpolate_value(300.0, 1300.0);
    if (state->input_manager->get_dir_up())
    {
        current_temperature_ratio += 0.1 * state->time_step;
        burner_on = true;
    }
    else
    {
        burner_on = false;
    }

    if (state->input_manager->get_dir_down())
    {
        current_temperature_ratio -= 0.1 * state->time_step;
        valve_open = true;
    }
    else
    {
        valve_open = false;
    }

    // Decay the current temperature
    current_temperature_ratio -= 0.02 * current_temperature_ratio * state->time_step;

    // Limit the current temperature value
    current_temperature_ratio = std::min(std::max(0.0, current_temperature_ratio), 1.0);

    // Setup lateral forces
    const double lat_force_max = 200.0;
    double lat_force = 0.0;
    if (state->input_manager->get_dir_right())
    {
        lat_force += lat_force_max;
    }
    if (state->input_manager->get_dir_left())
    {
        lat_force -= lat_force_max;
    }

    // Apply forces
    add_force_absolute(Vector2(lat_force, -vert_force));
}

bool Envelope::get_valve_open() const
{
    return valve_open;
}

bool Envelope::get_burner_on() const
{
    return burner_on;
}

double Envelope::get_temp_ratio() const
{
    return current_temperature_ratio;
}

Envelope::~Envelope()
{
    // Empty Destructor
}
