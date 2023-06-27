#include "balloon.h"

#include <world_state.h>

Balloon::Balloon() :
    rope_1(100.0),
    rope_2(100.0),
    rope_3(100.0),
    rope_4(100.0)
{
    // Add each item to the objects list
    objects.push_back(&gondola);
    objects.push_back(&envelope);

    objects.push_back(&rope_1);
    objects.push_back(&rope_2);

    objects.push_back(&rope_3);
    objects.push_back(&rope_4);

    objects.push_back(&weight_1);
    objects.push_back(&weight_2);

    // Setup the rope objects
    rope_1.set_object_a(&envelope);
    rope_1.set_object_b(&gondola);
    rope_2.set_object_a(&envelope);
    rope_2.set_object_b(&gondola);

    // Setup the weight rope objects
    rope_3.set_object_a(&gondola);
    rope_3.set_object_b(&weight_1);
    rope_4.set_object_a(&gondola);
    rope_4.set_object_b(&weight_2);
}

void Balloon::set_position(const double x, const double y)
{
    gondola.set_position(x, y);
    envelope.set_position(x, y - envelope.get_radius() * 2.0);

    const Vector2 weight_offset(0.0, 30.0);

    weight_1.set_position(gondola.get_bottom_left() + weight_offset);
    weight_2.set_position(gondola.get_bottom_right() + weight_offset);
}

void Balloon::draw(const DrawState* state)
{
    for (auto& obj : objects)
    {
        obj->draw(state);
    }
}

void Balloon::pre_step(const StepState* state)
{
    // Update rope broken parameters
    const WorldState* world_state = dynamic_cast<const WorldState*>(state);

    if (world_state->input_manager->get_key_rising_edge(ALLEGRO_KEY_1))
    {
        if (rope_3.get_broken())
        {
            rope_3.try_reattach_rope();
        }
        else
        {
            rope_3.break_rope();
        }
    }

    if (world_state->input_manager->get_key_rising_edge(ALLEGRO_KEY_2))
    {
        if (rope_4.get_broken())
        {
            rope_4.try_reattach_rope();
        }
        else
        {
            rope_4.break_rope();
        }
    }

    // Setup/update the rope points
    rope_1.set_point_a(envelope.anchor_point_left());
    rope_1.set_point_b(gondola.get_top_left());

    rope_2.set_point_a(envelope.anchor_point_right());
    rope_2.set_point_b(gondola.get_top_right());

    rope_3.set_point_a(gondola.get_bottom_left());
    rope_3.set_point_b(weight_1.get_position());

    rope_4.set_point_a(gondola.get_bottom_right());
    rope_4.set_point_b(weight_2.get_position());

    // Run each object pre-state
    for (auto& obj : objects)
    {
        obj->pre_step(state);
    }
}

void Balloon::step(const StepState* state)
{
    for (auto& obj : objects)
    {
        obj->step(state);
    }
}

void Balloon::post_step(const StepState* state)
{
    for (auto& obj : objects)
    {
        obj->post_step(state);
    }
}

const Envelope& Balloon::get_envelope() const
{
    return envelope;
}

const Gondola& Balloon::get_gondola() const
{
    return gondola;
}
