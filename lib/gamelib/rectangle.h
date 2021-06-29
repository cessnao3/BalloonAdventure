#ifndef GIO_RECTANGLE_H
#define GIO_RECTANGLE_H

#include "vector2.h"

#include <cstdint>

class Rectangle
{
public:
    Rectangle() :
        top_left_point(0.0, 0.0),
        width(0),
        height(0)
    {
        // Empty Constructor
    }

    Rectangle(const uint32_t width, const uint32_t height) :
        top_left_point(0.0, 0.0),
        width(width),
        height(height)
    {
        // Empty Constructor
    }

    uint32_t get_width() const
    {
        return width;
    }

    uint32_t get_height() const
    {
        return height;
    }

    Vector2 get_top_left() const
    {
        return top_left_point;
    }

    Vector2 get_top_right() const
    {
        return top_left_point + Vector2(static_cast<double>(width), 0.0);
    }

    Vector2 get_bottom_left() const
    {
        return top_left_point + Vector2(0.0, static_cast<double>(height));
    }

    Vector2 get_bottom_right() const
    {
        return top_left_point + Vector2(static_cast<double>(width), static_cast<double>(height));
    }

protected:
    Vector2 top_left_point;
    uint32_t width;
    uint32_t height;
};

#endif // GIO_RECTANGLE_H
