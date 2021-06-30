#include "balloon.h"

Balloon::Balloon() :
    rope_1(100.0),
    rope_2(100.0)
{
    // Add each item to the objects list
    objects.push_back(&gondola);
    objects.push_back(&envelope);
    objects.push_back(&rope_1);
    objects.push_back(&rope_2);

    // Setup the rope objects
    rope_1.set_object_a(&envelope);
    rope_1.set_object_b(&gondola);
    rope_2.set_object_a(&envelope);
    rope_2.set_object_b(&gondola);
}

void Balloon::set_position(const double x, const double y)
{
    gondola.set_position(x, y);
    envelope.set_position(x, y - envelope.get_radius() * 2.0);
}

void Balloon::draw(const DrawState* state)
{
    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        (*it)->draw(state);
    }
}

void Balloon::pre_step(const StepState* state)
{
    rope_1.set_point_a(envelope.anchor_point_left());
    rope_1.set_point_b(gondola.get_top_left());

    rope_2.set_point_a(envelope.anchor_point_right());
    rope_2.set_point_b(gondola.get_top_right());

    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        (*it)->pre_step(state);
    }
}

void Balloon::step(const StepState* state)
{
    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        (*it)->step(state);
    }
}

void Balloon::post_step(const StepState* state)
{
    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        (*it)->post_step(state);
    }
}
