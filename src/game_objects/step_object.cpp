#include "step_object.h"

void StepObject::pre_step(const StepState* state)
{
    for (size_t i = 0; i < other_step_objects.size(); ++i)
    {
        other_step_objects[i]->pre_step(state);
    }
}

void StepObject::step(const StepState* state)
{
    for (size_t i = 0; i < other_step_objects.size(); ++i)
    {
        other_step_objects[i]->step(state);
    }
}

void StepObject::post_step(const StepState* state)
{
    for (size_t i = 0; i < other_step_objects.size(); ++i)
    {
        other_step_objects[i]->step(state);
    }
}

bool StepObject::add_step_object(StepObject* obj)
{
    if (!contains_step_object(obj))
    {
        other_step_objects.push_back(obj);
        return true;
    }
    else
    {
        return false;
    }
}

bool StepObject::remove_step_object(const StepObject* obj)
{
    const std::vector<StepObject*>::const_iterator value = std::find(
        other_step_objects.begin(),
        other_step_objects.end(),
        obj);

    if (value != other_step_objects.end())
    {
        other_step_objects.erase(value, value + 1);
        return true;
    }
    else
    {
        return false;
    }
}

bool StepObject::contains_step_object(const StepObject* obj) const
{
    return other_step_objects.end() != std::find(
        other_step_objects.begin(),
        other_step_objects.end(),
        obj);
}
