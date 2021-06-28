#ifndef GONDOLA_H
#define GONDOLA_H

#include <gamelib/aero_object.h>
#include <gamelib/vector2.h>

#include <vector>
#include <allegro5/allegro_primitives.h>

class Gondola : public AeroObject
{
public:
    Gondola();

    Vector2 get_top_left() const;

    Vector2 get_top_right() const;

    Vector2 get_bottom_left() const;

    Vector2 get_bottom_right() const;

    void draw(const DrawState* state) override;

protected:
    double width;
    double height;
};

#endif // GONDOLA_H
