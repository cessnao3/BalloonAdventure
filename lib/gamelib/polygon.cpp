#include <gamelib/polygon.h>

Polygon::Polygon()
{
    // Empty Constructor
}

void Polygon::add_point(const double x, const double y)
{
    points.push_back(Vector2(x, y));
}

void Polygon::add_point(const Vector2& point)
{
    points.push_back(point);
}

void Polygon::clear()
{
    points.clear();
}

size_t Polygon::size() const
{
    return points.size();
}

const std::vector<Vector2>& Polygon::get_points() const
{
    return points;
}

std::vector<Vector2>& Polygon::get_points()
{
    return points;
}

std::vector<float> Polygon::get_allegro_points() const
{
    std::vector<float> al_points;

    for (auto it = points.begin(); it != points.end(); ++it)
    {
        al_points.push_back(static_cast<float>(it->x));
        al_points.push_back(static_cast<float>(it->y));
    }

    return al_points;
}
