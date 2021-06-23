#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <gamelib/input_manager.h>

class GameState
{
public:
    GameState()
    {
        // Empty Constructor
    }

public:
    bool get_running() const
    {
        return running;
    }

    void set_quit()
    {
        running = false;
    }

    InputManager* get_input_manager()
    {
        return &input_manager;
    }

private:
    bool running = true;
    InputManager input_manager;
};

#endif // BALLOON_GAME_STATE_H
