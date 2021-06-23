#ifndef GAME_STATE_H
#define GAME_STATE_H

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

private:
    bool running = true;
};

#endif // BALLOON_GAME_STATE_H
