#ifndef __IO_DRAW_OBJECT__
#define __IO_DRAW_OBJECT__

#include <vector>

#include <allegro5/allegro.h>

struct DrawState
{
    // Nothing... For Now!
};

class DrawObject
{
public:
    virtual void draw(const DrawState* state);

private:
    std::vector<DrawObject*> other_draw_objects;
};

#endif
