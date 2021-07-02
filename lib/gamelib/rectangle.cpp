#include <gamelib/rectangle.h>

Rectangle::Rectangle() :
    top_left_point(0.0, 0.0),
    width(0),
    height(0)
{
    // Empty Constructor
}

Rectangle::Rectangle(const uint32_t width, const uint32_t height) :
    top_left_point(0.0, 0.0),
    width(width),
    height(height)
{
    // Empty Constructor
}

uint32_t Rectangle::get_width() const
{
    return width;
}

uint32_t Rectangle::get_height() const
{
    return height;
}

Vector2 Rectangle::get_top_left() const
{
    return top_left_point;
}

Vector2 Rectangle::get_top_right() const
{
    return top_left_point + Vector2(static_cast<double>(width), 0.0);
}

Vector2 Rectangle::get_bottom_left() const
{
    return top_left_point + Vector2(0.0, static_cast<double>(height));
}

Vector2 Rectangle::get_bottom_right() const
{
    return top_left_point + Vector2(static_cast<double>(width), static_cast<double>(height));
}
