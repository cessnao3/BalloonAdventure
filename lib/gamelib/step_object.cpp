#include "step_object.h"

StepState::StepState() :
    time_step(0.0),
    input_manager(nullptr)
{
    // Empty Constructor
}

StepState::~StepState()
{
    // Do Nothing;
}

void StepObject::pre_step(const StepState* state)
{
    // Do Nothing
}

void StepObject::step(const StepState* state)
{
    // Do Nothing
}

void StepObject::post_step(const StepState* state)
{
    // Do Nothing
}
