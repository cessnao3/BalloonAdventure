#ifndef GIO_POLYGON_H
#define GIO_POLYGON_H

#include <cstddef>
#include <vector>

#include <gamelib/vector2.h>

class Polygon
{
public:
    Polygon();

    void add_point(
        const double x,
        const double y);

    void add_point(const Vector2& point);

    void clear();

    size_t size() const;

    const std::vector<Vector2>& get_points() const;

    std::vector<Vector2>& get_points();

    std::vector<float> get_allegro_points() const;

protected:
    std::vector<Vector2> points;
};

#endif // GIO_POLYGON_H
