#ifndef __IO_GAME_OBJECT__
#define __IO_GAME_OBJECT__

#include <vector>

struct StepState
{
    double time_step;
};

class StepObject
{
public:
    virtual void pre_step(const StepState* state);

    virtual void step(const StepState* state);

    virtual void post_step(const StepState* state);

protected:
    bool add_step_object(StepObject* obj);

    bool remove_step_object(const StepObject* obj);

    bool contains_step_object(const StepObject* obj) const;

private:
    std::vector<StepObject*> other_step_objects;
};

#endif
