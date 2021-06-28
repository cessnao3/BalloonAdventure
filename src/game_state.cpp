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

    // Set the balloon position
    balloon.set_position(
        draw_state.window.get_width() / 2.0,
        draw_state.window.get_height() / 2.0);

    // Define the step state
    world_state.input_manager = get_input_manager();
    world_state.time_step = 0.001;
    world_state.gravity = Vector2(0.0, 10.0);
    world_state.terrain = &terrain;

    // Add the terrain as a draw object
    draw_objects.push_back(&terrain);

    // Add the balloon parameters
    draw_objects.push_back(&balloon);
    step_objects.push_back(&balloon);
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
            (*it)->pre_step(&world_state);
        }

        for (auto it = step_objects.begin(); it != step_objects.end(); ++it)
        {
            (*it)->step(&world_state);
        }

        for (auto it = step_objects.begin(); it != step_objects.end(); ++it)
        {
            (*it)->step(&world_state);
        }

        // Increment the current time
        t += world_state.time_step;
    }
}
