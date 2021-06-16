#ifndef GIO_GAME_OBJECT_H
#define GIO_GAME_OBJECT_H

#include "draw_object.h"
#include "step_object.h"

class GameObject : public StepObject, public DrawObject
{

};

#endif // GIO_GAME_OBJECT_H
