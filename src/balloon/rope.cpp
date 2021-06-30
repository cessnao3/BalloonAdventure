#include "rope.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <cmath>

Rope::Rope(const double spring_constant) :
    spring_constant(spring_constant),
    init_length(-1.0)
{
    obj_a = nullptr;
    obj_b = nullptr;
}

void Rope::set_object_a(PhysicsObject* obj)
{
    obj_a = obj;
    init_length = -1.0;
}

void Rope::set_object_b(PhysicsObject* obj)
{
    obj_b = obj;
    init_length = -1.0;
}

void Rope::set_point_a(const Vector2& p)
{
    point_a = p;
}

void Rope::set_point_b(const Vector2& p)
{
    point_b = p;
}

void Rope::draw(const DrawState* state)
{
    // Draw the line
    al_draw_line(
        static_cast<float>(point_a.x),
        static_cast<float>(point_a.y),
        static_cast<float>(point_b.x),
        static_cast<float>(point_b.y),
        al_map_rgb(0, 0, 0),
        2.0f);
}

void Rope::pre_step(const StepState* state)
{
    // Run the super state
    GameObject::pre_step(state);

    // Update the initial length if required
    if (init_length < 0.0)
    {
        init_length = (point_a - point_b).magnitude();
    }

    // Determine the spring force
    const double spring_force = spring_constant * std::max(((point_a - point_b).magnitude() - init_length), 0.0);
    
    const Vector2 offset_a = point_a - obj_a->get_position();
    const Vector2 offset_b = point_b - obj_b->get_position();

    const Vector2 force_dir = (point_b - point_a).normalize();

    // Apply forces to the respective objects
    obj_a->add_force_absolute(force_dir * spring_force, offset_a);
    obj_b->add_force_absolute(force_dir * -spring_force, offset_b);

    /*
    self.point_a.obj.add_force(
        force = rope_force_dir,
        offset = self.point_a.offset())
        self.point_b.obj.add_force(
            force = rope_force_dir * -1.0,
            offset = self.point_b.offset())
    */
}
