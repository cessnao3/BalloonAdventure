#ifndef GIO_DRAW_OBJECT_H
#define GIO_DRAW_OBJECT_H

#include "vector2.h"

struct DrawState
{
    Vector2 draw_offset;
};

class DrawObject
{
public:
    virtual void draw(const DrawState* state);
};

#endif // GIO_DRAW_OBJECT_H
