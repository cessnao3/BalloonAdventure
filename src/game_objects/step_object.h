#pragma once

struct StepState
{
    double time_step;

    virtual ~StepState()
    {
        // Do Nothing;
    }
};

class StepObject
{
public:
    virtual void pre_step(const StepState* state);

    virtual void step(const StepState* state);

    virtual void post_step(const StepState* state);
};
