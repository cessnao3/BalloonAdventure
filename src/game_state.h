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
#include <world_state.h>
#include <menu_state_flow.h>

#include <sound_manager.h>

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

    /**
     * @brief Initializes any non-constructor init items
     * @return true if successfully initialized
     */
    bool init();

    /**
     * @brief sets the stored display reference to draw with
     * @param display sets the display
     */
    void set_display(ALLEGRO_DISPLAY* display);

    /**
     * @brief provides the internally-used screen width
     * @return screen width
     */
    size_t get_screen_width() const;

    /**
     * @brief provides the internally-used screen height
     * @return screen height
     */
    size_t get_screen_height() const;

    /**
     * @brief Sets the screen size to a new value
     * @param width the new width to set
     * @param height the new height to set
     */
    void set_screen_size(
        const size_t width,
        const size_t height);

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
     * @brief provides the core sound manager for the game state
     * @return a poionter to the sound manager
    */
    SoundManager* get_sound_manager();

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
    InputManager input_manager_autopilot;

    SoundManager sound_manager;

    DrawState draw_state;
    WorldState world_state;

    MenuStateFlow menu_state_flow;

    Terrain terrain;

    Balloon balloon;
};

#endif // BALLOON_GAME_STATE_H
