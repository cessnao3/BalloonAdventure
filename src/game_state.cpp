#include "game_state.h"

GameState::GameState(ALLEGRO_DISPLAY* display)
{
    // Define the draw state
    draw_state.draw_offset = Vector2();
    draw_state.display = display;

    // Set the balloon position
    balloon.set_position(
        al_get_display_width(display) / 2.0,
        al_get_display_height(display) / 2.0);

    // Define the step state
    world_state.input_manager = get_input_manager();
    world_state.time_step = 0.0001;
    world_state.gravity = Vector2(0.0, 10.0);
    world_state.terrain = &terrain;

    // Add the terrain as a draw object
    draw_objects.push_back(&terrain);

    // Add the balloon parameters
    draw_objects.push_back(&balloon);
    step_objects.push_back(&balloon);
}

bool GameState::init()
{
    return sound_manager.init();
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

SoundManager* GameState::get_sound_manager()
{
    return &sound_manager;
}

void GameState::draw()
{
    // Update the window offset if needed
    Vector2 diff_val = balloon.get_gondola().get_position() - draw_state.draw_offset;

    const double display_width = al_get_display_width(draw_state.display);
    const double display_height = al_get_display_height(draw_state.display);

    const double lim_x_right = 0.8 * display_width;
    const double lim_x_left = 0.2 * display_width;

    const double lim_y_top = 0.4 * display_height;
    const double lim_y_bot = 0.8 * display_height;

    if (diff_val.x > lim_x_right)
    {
        draw_state.draw_offset.x += diff_val.x - lim_x_right;
    }
    else if (diff_val.x < lim_x_left)
    {
        draw_state.draw_offset.x += diff_val.x - lim_x_left;
    }

    if (diff_val.y < lim_y_top)
    {
        draw_state.draw_offset.y += diff_val.y - lim_y_top;
    }
    else if (diff_val.y > lim_y_bot)
    {
        draw_state.draw_offset.y += diff_val.y - lim_y_bot;
    }

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

    // Check for a button press to stop any music
    if (input_manager.get_key_rising_edge(ALLEGRO_KEY_M))
    {
        sound_manager.set_music_state(!sound_manager.get_music_state());
    }

    // Update sounds
    sound_manager.set_burner_state(balloon.get_envelope().get_burner_on() ? SoundManager::BurnerState::ON : SoundManager::BurnerState::OFF);
    sound_manager.set_valve_state(balloon.get_envelope().get_valve_open() ? SoundManager::ValveState::OPEN : SoundManager::ValveState::CLOSED);
    sound_manager.update_background();
}

void GameState::step(const double dt)
{
    // Run all steps
    const size_t num_steps = static_cast<size_t>(dt / world_state.time_step);

    // Run through the timestep to perform the integration in smaller timesteps
    // to help maintain system stability
    for (size_t i = 0; i < num_steps; ++i)
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
            (*it)->post_step(&world_state);
        }
    }
}
