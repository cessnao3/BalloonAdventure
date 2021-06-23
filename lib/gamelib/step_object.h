#ifndef GIO_STEP_OBJECT_H
#define GIO_STEP_OBJECT_H

/**
 * @brief Provides the basic state information for a given step
 */
struct StepState
{
    double time_step;

    virtual ~StepState();
};

/**
 * @brief Provides the base object for a steppable object
 */
class StepObject
{
public:
    /**
     * @brief function to run right before the step
     * @param state is the state to use for the step computation
     */
    virtual void pre_step(const StepState* state);

    /**
     * @brief function to run to perform the step
     * @param state is the state to use for the step computation
     */
    virtual void step(const StepState* state);

    /**
     * @brief function to run right after the step
     * @param state is the state to use for the step computation
     */
    virtual void post_step(const StepState* state);
};

#endif // GIO_STEP_OBJECT_H
