#ifndef TERRAIN_H
#define TERRAIN_H

#include <gamelib/draw_object.h>
#include <gamelib/vector2.h>

#include <gamelib/polygon.h>

class Terrain : public DrawObject
{
public:
    Terrain();

    virtual void draw(const DrawState* state) override;

    double elevation_at_x(const double x);

    Vector2 surface_normal_at_x(const double x);

protected:
    double base_height = 50.0;

    double spring_constant = 500.0;
    double damping_coefficient = 250.0;
    double friction_damping = 100.0;

    double base_amplitude = 20.0;
    double base_frequency = 0.01;

private:
    Polygon terrain_points;
};

#endif // TERRAIN_H
