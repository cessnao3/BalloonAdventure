#ifndef GIO_DRAW_OBJECT_H
#define GIO_DRAW_OBJECT_H

#include <gamelib/vector2.h>`
#include <gamelib/input_manager.h>

#include <allegro5/display.h>

/**
 * @brief a basic drawing state to use when drawing
 */
struct DrawState
{
    Vector2 draw_offset = Vector2();
    ALLEGRO_DISPLAY* display = nullptr;
    InputManager* input_manager = nullptr;
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
