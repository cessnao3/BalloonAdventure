#ifndef GIO_PHYSICS_OBJECT_H
#define GIO_PHYSICS_OBJECT_H

#include <gamelib/game_object.h>

#include <gamelib/vector2.h>


/**
 * @brief provides the core physics state information to pass to an object
*/
struct PhysicsState : public StepState
{
    Vector2 gravity;
};

/**
 * @brief provides a basic physics object to use within a game
*/
class PhysicsObject : public GameObject
{
public:
    /**
     * @brief constructs the basic physics object
    */
    PhysicsObject();

    /**
     * @brief Applies a relative force to the object
     * @param force the force to apply
    */
    void add_force_relative(const Vector2& force);

    /**
     * @brief Applies a relative force to the object
     * @param force the force to apply
     * @param offset the offset from the center of mass to apply the force at, relative to the body
    */
    void add_force_relative(
        const Vector2& force,
        const Vector2& offset);

    /**
     * @brief Applies a force to the object in the global frame
     * @param force the force to apply
    */
    void add_force_absolute(const Vector2& force);

    /**
     * @brief Applies a relative force to the object
     * @param force the force to apply
     * @param offset the offset from the center of mass to apply the force at, from the global frame
    */
    void add_force_absolute(
        const Vector2& force,
        const Vector2& offset);

    /**
     * @brief Steps the core physics state
     * @param state the step (physics) state to use
    */
    virtual void step(const StepState* state) override;

    /**
     * @brief Runs after the main step
     * @param state the step state to use
     */
    virtual void post_step(const StepState* state) override;

    /**
     * @brief Resets all forces and moments within the physics object to 0
     */
    virtual void reset_forces();

    /**
     * @brief Sets the position to the position given
     * @param pos the new position to set
     */
    void set_position(const Vector2& pos);

    /**
     * @brief Sets the position to the position given
     * @param x the x coordinate
     * @param y the y coordinate
     */
    void set_position(const double x, const double y);

    /**
     * @brief Provides the current object position
     * @return the current position
     */
    Vector2 get_position() const;

    /**
     * @brief Provides the current object central velocity
     * @return the current velocity
     */
    Vector2 get_velocity() const;

    /**
     * @brief Provides the current object velocity at the point requested
     * @param point the other point to check the velocity at in absolute coordinates
     * @return the current velocity at the point provided
     */
    Vector2 get_velocity_at_absolute(const Vector2& point);

protected:
    Vector2 position;
    Vector2 velocity;
    
    double rotation;
    double rotational_vel;

    double mass;
    double inertia;

    Vector2 forces;
    double moments;
};

#endif // GIO_PHYSICS_OBJECT_H
