#ifndef GIO_DRAW_OBJECT_H
#define GIO_DRAW_OBJECT_H

struct DrawState
{
    // Nothing... For Now!
};

class DrawObject
{
public:
    virtual void draw(const DrawState* state);
};

#endif // GIO_DRAW_OBJECT_H
