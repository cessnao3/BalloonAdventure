#ifndef GIO_AERO_OBJECT_H
#define GIO_AERO_OBJECT_H

#include "physics_object.h"

#include <cmath>

/**
 * @brief Provides basic aerodynamics forces for a physics object
 */
class AeroObject : public PhysicsObject
{
public:
    /**
     * @brief constructs the aerodynamics object
     * @param cd the drag coefficient to use
    */
    AeroObject(const double cd);

    /**
     * @brief state to run before the physics step to setup the aerodynamics forces
     * @param state the step state to use for computation
    */
    virtual void pre_step(const StepState* state) override;

protected:
    double cd;
};

#endif
