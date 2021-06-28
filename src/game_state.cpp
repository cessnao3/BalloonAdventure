#include "game_state.h"

GameState::GameState()
{
    // Obtain the current window
    ALLEGRO_DISPLAY* display = al_get_current_display();

    // Define the draw state
    draw_state.draw_offset = Vector2();
    draw_state.window = Rectangle(
        static_cast<size_t>(al_get_display_width(display)),
        static_cast<size_t>(al_get_display_height(display)));

    // Define the step state
    step_state.input_manager = get_input_manager();
    step_state.time_step = 0.001;

    // Add the terrain as a draw object
    draw_objects.push_back(&terrain);
}

bool GameState::get_running() const
{
    return running;
}

void GameState::set_quit()
{
    running = false;
}

InputManager* GameState::get_input_manager()
{
    return &input_manager;
}

void GameState::draw()
{
    // Define and set the background color
    const ALLEGRO_COLOR background_color = al_map_rgb(124, 199, 231);
    al_clear_to_color(background_color);

    // Run all drawable parameters
    for (auto it = draw_objects.begin(); it != draw_objects.end(); ++it)
    {
        (*it)->draw(&draw_state);
    }

    // Flip the screen
    al_flip_display();
}

void GameState::step(const double dt)
{
    // Run all steps
    double t = 0.0;

    // Run through the timestep to perform the integration in smaller timesteps
    // to help maintain system stability
    while (t < dt)
    {
        // Run each pre, step, and post function
        for (auto it = step_objects.begin(); it != step_objects.end(); ++it)
        {
            (*it)->pre_step(&step_state);
        }

        for (auto it = step_objects.begin(); it != step_objects.end(); ++it)
        {
            (*it)->step(&step_state);
        }

        for (auto it = step_objects.begin(); it != step_objects.end(); ++it)
        {
            (*it)->step(&step_state);
        }

        // Increment the current time
        t += step_state.time_step;
    }
}
