#ifndef GIO_POLYGON_H
#define GIO_POLYGON_H

#include <cstddef>
#include <vector>

#include "vector2.h"

class Polygon
{
public:
    Polygon()
    {
        // Empty Constructor
    }

    void add_point(const double x, const double y)
    {
        points.push_back(Vector2(x, y));
    }

    void add_point(const Vector2& point)
    {
        points.push_back(point);
    }

    void clear()
    {
        points.clear();
    }

    size_t size() const
    {
        return points.size();
    }

    const std::vector<Vector2>& get_points() const
    {
        return points;
    }

    std::vector<Vector2>& get_points()
    {
        return points;
    }

    std::vector<float> get_allegro_points() const
    {
        std::vector<float> al_points;

        for (auto it = points.begin(); it != points.end(); ++it)
        {
            al_points.push_back(static_cast<float>(it->x));
            al_points.push_back(static_cast<float>(it->y));
        }

        return al_points;
    }

protected:
    std::vector<Vector2> points;
};

#endif // GIO_POLYGON_H
