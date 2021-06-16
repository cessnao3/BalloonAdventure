#include "draw_object.h"

void DrawObject::draw(const DrawState* state)
{
    for (size_t i = 0; i < other_draw_objects.size(); ++i)
    {
        other_draw_objects[i]->draw(state);
    }
}
