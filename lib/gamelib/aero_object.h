#ifndef GIO_AERO_OBJECT_H
#define GIO_AERO_OBJECT_H

#include "physics_object.h"

/**
 * @brief Provides basic aerodynamics forces for a physics object
 */
class AeroObject : public PhysicsObject
{
public:
    /**
     * @brief construcst the aerodynamics object
     * @param cd_translation the translational drag coefficient to use
     * @param cd_rotation the rotational drag coefficient to use
     */
    AeroObject(
        const double cd_translation,
        const double cd_rotation);

    /**
     * @brief state to run before the physics step to setup the aerodynamics forces
     * @param state the step state to use for computation
    */
    virtual void pre_step(const StepState* state) override;

protected:
    double cd_translation;
    double cd_rotation;
};

#endif
