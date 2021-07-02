#include "aero_object.h"

#include <cmath>

AeroObject::AeroObject(
    const double cd_translation,
    const double cd_rotation) :
    cd_translation(cd_translation),
    cd_rotation(cd_rotation)
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
        const Vector2 drag = -0.5 * cd_translation * vm2 * velocity.normalize();
        add_force_absolute(drag);
    }
    
    // Apply rotational drag
    if (std::abs(rotational_vel) > 1e-6)
    {
        moments -= 0.5 * rotational_vel * std::abs(rotational_vel) * cd_rotation;
    }
}
