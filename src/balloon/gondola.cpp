#include "gondola.h"

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

void Gondola::draw(const DrawState* state)
{
    // Define the core points
    const std::vector<Vector2> points = {
        get_top_left(),
        get_bottom_left(),
        get_bottom_right(),
        get_top_right()
    };

    // Add each point to a vector for use in drawing
    std::vector<float> al_points;

    for (auto it = points.begin(); it != points.end(); ++it)
    {
        al_points.push_back(static_cast<float>(it->x));
        al_points.push_back(static_cast<float>(it->y));
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
