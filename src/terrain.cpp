#include "terrain.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <gamelib/polygon.h>

#include <cmath>
#include <vector>

Terrain::Terrain() :
    bitmap(nullptr)
{
    // Empty Constructor
}

void Terrain::draw(const DrawState* state)
{
    // Update the bitmap if needed
    if (bitmap == nullptr)
    {
        // Extract the height and width
        const int display_width = al_get_display_width(state->display);
        const int display_height = al_get_display_height(state->display);

        // Update the bitmap
        ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();
        bitmap = al_create_bitmap(
            display_width,
            display_height);
        al_set_target_bitmap(bitmap);

        // Clear the resulting points
        terrain_points.clear();

        // Define points
        const Vector2 display_bottom_left(
            0.0,
            static_cast<double>(display_height));
        const Vector2 display_bottom_right(
            static_cast<double>(display_width),
            static_cast<double>(display_height));

        // Add the bottom-right surface
        terrain_points.add_point(display_bottom_right);

        // Iterate over each point to assign to the results
        for (int i = display_width; i >= 0; --i)
        {
            // Define the inner-X location
            terrain_points.add_point(
                Vector2(
                    i + state->draw_offset.x,
                    elevation_at_x(i + state->draw_offset.x)) - state->draw_offset);
        }

        // Add the final point
        terrain_points.add_point(display_bottom_left);

        // Obtain the allegro points to use
        const std::vector<float> al_points = terrain_points.get_allegro_points();

        // Draw the terrain polygon
        al_draw_filled_polygon(
            al_points.data(),
            static_cast<int>(terrain_points.size()),
            al_map_rgb(50, 150, 75));

        // Reset the draw target
        al_set_target_bitmap(prev_target);
    }

    // Draw the bitmap
    al_draw_bitmap(
        bitmap,
        0,
        0,
        0);
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

Terrain::~Terrain()
{
    if (bitmap != nullptr)
    {
        al_destroy_bitmap(bitmap);
        bitmap = nullptr;
    }
}
