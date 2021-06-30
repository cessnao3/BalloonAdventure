#ifndef BALLOON_H
#define BALLOON_H

#include <gamelib/game_object.h>

#include "gondola.h"
#include "envelope.h"
#include "rope.h"

#include <vector>

class Balloon : public GameObject
{
public:
    Balloon();

    void set_position(const double x, const double y);

    virtual void draw(const DrawState* state) override;

    virtual void pre_step(const StepState* state) override;

    virtual void step(const StepState* state) override;

    virtual void post_step(const StepState* state) override;

protected:
    Gondola gondola;
    Envelope envelope;

    Rope rope_1;
    Rope rope_2;

    std::vector<GameObject*> objects;
};

#endif // BALLOON_H
