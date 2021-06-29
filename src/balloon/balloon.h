#ifndef BALLOON_H
#define BALLOON_H

#include <gamelib/game_object.h>

#include "gondola.h"

class Balloon : public GameObject
{
public:
    Balloon()
    {
        // Empty Constructor
    }

    void set_position(const double x, const double y)
    {
        gondola.set_position(x, y);
    }

    virtual void draw(const DrawState* state) override
    {
        gondola.draw(state);
    }

    virtual void pre_step(const StepState* state) override
    {
        gondola.pre_step(state);
    }

    virtual void step(const StepState* state) override
    {
        gondola.step(state);
    }

    virtual void post_step(const StepState* state) override
    {
        gondola.post_step(state);
    }

protected:
    Gondola gondola;
};

#endif // BALLOON_H
