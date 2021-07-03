#include "terrain.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <gamelib/polygon.h>

#include <cmath>
#include <vector>

Terrain::Terrain() :
    bitmap(nullptr)
{
    // Initialize constants
    base_height = 650.0;

    spring_constant = 500.0;
    damping_coefficient = 250.0;
    friction_damping = 100.0;

    base_amplitude = 20.0;
    base_frequency = 0.01;
}

void Terrain::draw(const DrawState* state)
{
    // Extract the height and width
    const int display_width = static_cast<int>(state->screen_w);
    const int display_height = static_cast<int>(state->screen_h);

    const double display_width_d = static_cast<double>(display_width);
    const double display_height_d = static_cast<double>(display_height);

    // Define the current draw offset difference
    const Vector2 offset_diff = state->draw_offset - bitmap_offset;
    const double tol_thresh = 0.95;
    const bool x_requires_redraw = offset_diff.x < -tol_thresh * display_width || offset_diff.x > tol_thresh * display_width;
    const bool y_requires_redraw = offset_diff.y < -tol_thresh * display_height || offset_diff.y > tol_thresh * display_height;

    const bool redraw_bitmap = bitmap == nullptr || x_requires_redraw || y_requires_redraw;

    // Update the bitmap if needed
    if (redraw_bitmap)
    {
        // Create the bitmap if needed
        if (bitmap == nullptr)
        {
            bitmap = al_create_bitmap(
                display_width * 3,
                display_height * 3);
        }

        // Update the bitmap
        ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();
        al_set_target_bitmap(bitmap);

        //al_clear_to_color(al_map_rgba(0, 0, 0, 0));

        bitmap_offset = state->draw_offset;

        // Clear the resulting points
        terrain_points.clear();

        const Vector2 draw_offset = Vector2(
            -display_width_d,
            -display_height_d);

        // Define points
        const Vector2 display_bottom_left = Vector2(
            -display_width_d,
            2.0 * display_height_d) - draw_offset;
        const Vector2 display_bottom_right = Vector2(
            2.0 * display_width_d,
            2.0 * display_height_d) - draw_offset;

        // Add the bottom-right surface
        terrain_points.add_point(display_bottom_right);

        // Iterate over each point to assign to the results
        for (int i = 2 * display_width; i >= -display_width; --i)
        {
            // Define the inner-X location
            terrain_points.add_point(
                Vector2(
                    i + bitmap_offset.x,
                    elevation_at_x(i + state->draw_offset.x)) - bitmap_offset - draw_offset);
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
        bitmap_offset.x - state->draw_offset.x - display_width_d,
        bitmap_offset.y - state->draw_offset.y - display_height_d,
        0);
}

void Terrain::invalidate_draw(const DrawState* state)
{
    if (bitmap != nullptr)
    {
        al_destroy_bitmap(bitmap);
        bitmap = nullptr;
    }
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
