#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <memory>
#include <vector>

#include <gamelib/input_manager.h>
#include <gamelib/draw_object.h>
#include <gamelib/step_object.h>

#include <allegro5/allegro.h>

#include <balloon/balloon.h>

#include <terrain.h>

/**
 * @brief Defines basic state information for the overall game state
 */
class GameState
{
public:
    /**
     * @brief Constructs the game state
     */
    GameState();

public:
    /**
     * @brief provides information for whether the game is currently running
     * @return true if the game is marked to continue to run
     */
    bool get_running() const;

    /**
     * @brief marks the game to trigger a quit event
     */
    void set_quit();

    /**
     * @brief provides the core input manager for the game state
     * @return a pointer to the game input manager
     */
    InputManager* get_input_manager();

    /**
     * @brief runs the draw algorithm for all drawable parameters
     */
    void draw();

    /**
     * @brief runs the step algorithm for all steppable parameters
     * @param dt provides the delta time since the last step call
     */
    void step(const double dt);

private:
    std::vector<DrawObject*> draw_objects;
    std::vector<StepObject*> step_objects;

    bool running = true;
    InputManager input_manager;

    DrawState draw_state;
    StepState step_state;

    Terrain terrain;

    Balloon balloon;
};

#endif // BALLOON_GAME_STATE_H
