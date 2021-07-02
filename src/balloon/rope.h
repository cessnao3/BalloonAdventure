#ifndef ROPE_H
#define ROPE_H

#include <gamelib/vector2.h>
#include <gamelib/physics_object.h>

class Rope : public GameObject
{
public:
    Rope(const double spring_constant);

    void set_object_a(PhysicsObject* obj);

    void set_object_b(PhysicsObject* obj);

    void set_point_a(const Vector2& p);

    void set_point_b(const Vector2& p);

    void break_rope();

    bool get_broken() const;

    bool try_reattach_rope();

    double get_init_length() const;

    virtual void draw(const DrawState* state) override;

    virtual void pre_step(const StepState* state) override;

protected:
    double spring_constant;
    double init_length;

    bool broken;

    PhysicsObject* obj_a;
    PhysicsObject* obj_b;

    Vector2 point_a;
    Vector2 point_b;
};

#endif // ROPE_H
