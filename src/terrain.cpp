#include "terrain.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <gamelib/polygon.h>

#include <cmath>
#include <vector>

Terrain::Terrain()
{
    // Empty Constructor
}

void Terrain::draw(const DrawState* state)
{
    // Clear the resulting points
    terrain_points.clear();

    // Add the bottom-left surface
    terrain_points.add_point(state->window.get_bottom_right());

    // Define the window width
    const int window_width = static_cast<int>(state->window.get_width());

    // Iterate over each point to assign to the results
    for (int i = window_width; i >= 0; --i)
    {
        // Define the inner-X location
        terrain_points.add_point(
            Vector2(
                i + state->draw_offset.x,
                elevation_at_x(i + state->draw_offset.x)) - state->draw_offset);
    }

    // Add the final point
    terrain_points.add_point(state->window.get_bottom_left());

    // Obtain the allegro points to use
    const std::vector<float> al_points = terrain_points.get_allegro_points();

    // Draw the terrain polygon
    al_draw_filled_polygon(
        al_points.data(),
        static_cast<int>(terrain_points.size()),
        al_map_rgb(50, 150, 75));
}

double Terrain::elevation_at_x(const double x)
{
    return base_height + base_amplitude * std::sin(base_frequency * x);
}

Vector2 Terrain::surface_normal_at_x(const double x)
{
    return Vector2(
        base_amplitude * base_frequency * std::cos(base_frequency * x),
        -1.0).normalize();
}

double Terrain::get_spring_constant() const
{
    return 700.0;
}

double Terrain::get_damping_coefficient() const
{
    return 350.0;
}

double Terrain::get_frictional_cofficient() const
{
    return 100.0;
}
