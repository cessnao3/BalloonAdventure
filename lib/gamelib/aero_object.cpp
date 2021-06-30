#include "aero_object.h"

#include <cmath>

AeroObject::AeroObject(const double cd) :
    cd(cd)
{
    // Empty Constructor
}

void AeroObject::pre_step(const StepState* state)
{
    // Run the super pre-step
    PhysicsObject::pre_step(state);

    // Get the aerodynamic velocity squared
    const double vm2 = velocity.magnitude_squared();

    // Apply drag in the opposite direction if the magnitude is great enough
    if (std::abs(vm2) > 1e-6)
    {
        const Vector2 drag = -0.5 * cd * vm2 * velocity.normalize();
        add_force_absolute(drag);
    }
    
    // Apply rotational drag
    moments -= 0.5 * rotational_vel * rotational_vel * cd;
}
