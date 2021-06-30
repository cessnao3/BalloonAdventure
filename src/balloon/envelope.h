#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <allegro5/bitmap.h>

#include <gamelib/aero_object.h>
#include <gamelib/vector2.h>

class Envelope : public AeroObject
{
public:
    Envelope();

    Vector2 anchor_point_left() const;

    Vector2 anchor_point_right() const;

    double get_radius() const;

    virtual void pre_step(const StepState* state) override;

    virtual void draw(const DrawState* state) override;

    ~Envelope();

protected:
    double radius;
    ALLEGRO_BITMAP* bitmap;
};

#endif // ENVELOPE_H
