#pragma once

struct DrawState
{
    // Nothing... For Now!
};

class DrawObject
{
public:
    virtual void draw(const DrawState* state);
};
