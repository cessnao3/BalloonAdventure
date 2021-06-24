#ifndef GIO_DRAW_OBJECT_H
#define GIO_DRAW_OBJECT_H

#include "vector2.h"

/**
 * @brief a basic drawing state to use when drawing
 */
struct DrawState
{
    Vector2 draw_offset;
};

/**
 * @brief a base class for a drawable object
 */
class DrawObject
{
public:
    /**
     * @brief draws the current object onto the surface
     * @param state the draw state to use
     */
    virtual void draw(const DrawState* state);

    /**
     * @brief virtual destructor
    */
    virtual ~DrawObject();
};

#endif // GIO_DRAW_OBJECT_H
