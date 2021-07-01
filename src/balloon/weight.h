#ifndef WEIGHT_H
#define WEIGHT_H

#include <gamelib/aero_object.h>

class Weight : public AeroObject
{
public:
    Weight();

    virtual void draw(const DrawState* state) override;

    virtual void pre_step(const StepState* state) override;

protected:
    double radius;
};

#endif // WEIGHT_H
