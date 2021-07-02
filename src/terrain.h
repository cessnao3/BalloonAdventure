#ifndef TERRAIN_H
#define TERRAIN_H

#include <gamelib/draw_object.h>
#include <gamelib/vector2.h>

#include <gamelib/polygon.h>

#include <allegro5/allegro.h>

class Terrain : public DrawObject
{
public:
    Terrain();

    virtual void draw(const DrawState* state) override;

    double elevation_at_x(const double x);

    Vector2 surface_normal_at_x(const double x);

    double get_spring_constant() const;

    double get_damping_coefficient() const;

    double get_frictional_cofficient() const;

    ~Terrain();

protected:
    double base_height;

    double spring_constant;
    double damping_coefficient;
    double friction_damping;

    double base_amplitude;
    double base_frequency;

private:
    Polygon terrain_points;
    Vector2 bitmap_offset;
    ALLEGRO_BITMAP* bitmap;
};

#endif // TERRAIN_H
