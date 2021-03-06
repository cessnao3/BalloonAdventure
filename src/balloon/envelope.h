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

    double interpolate_value(const double min_val, const double max_val) const;

    bool get_valve_open() const;

    bool get_burner_on() const;

    double get_temp_ratio() const;

    ~Envelope();

protected:
    double radius;

    double current_temperature_ratio;

    bool burner_on;
    bool valve_open;
};

#endif // ENVELOPE_H
