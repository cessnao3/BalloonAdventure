#ifndef GIO_RECTANGLE_H
#define GIO_RECTANGLE_H

#include <gamelib/vector2.h>

#include <cstdint>

class Rectangle
{
public:
    Rectangle();

    Rectangle(
        const uint32_t width,
        const uint32_t height);

    uint32_t get_width() const;

    uint32_t get_height() const;

    Vector2 get_top_left() const;

    Vector2 get_top_right() const;

    Vector2 get_bottom_left() const;

    Vector2 get_bottom_right() const;

protected:
    Vector2 top_left_point;
    uint32_t width;
    uint32_t height;
};

#endif // GIO_RECTANGLE_H
