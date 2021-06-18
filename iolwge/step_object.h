#ifndef GIO_STEP_OBJECT_H
#define GIO_STEP_OBJECT_H

struct StepState
{
    double time_step;

    virtual ~StepState();
};

class StepObject
{
public:
    virtual void pre_step(const StepState* state);

    virtual void step(const StepState* state);

    virtual void post_step(const StepState* state);
};

#endif // GIO_STEP_OBJECT_H
