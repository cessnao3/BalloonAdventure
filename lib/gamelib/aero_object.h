#ifndef GIO_AERO_OBJECT_H
#define GIO_AERO_OBJECT_H

#include "physics_object.h"

#include <cmath>

class AeroObject : public PhysicsObject
{
public:
    virtual void pre_step(const StepState* state) override
    {
        // Get the aerodynamic velocity squared
        const double vm2 = velocity.magnitude_squared();

        // Apply drag in the opposite direction if the magnitude is great enough
        if (std::abs(vm2) > 1e-6)
        {
            const Vector2 drag = -0.5 * cd * vm2 * velocity.normalize();
            add_force_absolute(drag);
        }
    }

protected:
    double cd = 0.0;
};

#endif
