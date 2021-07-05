#include "game_state.h"

#include <allegro5/allegro_primitives.h>

#include <allegro5/allegro_audio.h>

GameState::GameState()
{
    // Define the draw state
    draw_state.draw_offset = Vector2();
    draw_state.display = nullptr;
    draw_state.input_manager = get_input_manager();
    draw_state.screen_w = 1280;
    draw_state.screen_h = 720;

    // Set the balloon position
    balloon.set_position(
        static_cast<double>(draw_state.screen_w) / 2.0,
        static_cast<double>(draw_state.screen_h) / 2.0);

    // Define the step state
    world_state.input_manager = &input_manager_autopilot;
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
    return
        sound_manager.init() &&
        menu_state_flow.init(&draw_state);
}

void GameState::set_display(ALLEGRO_DISPLAY* display)
{
    draw_state.display = display;
    for (auto it = draw_objects.begin(); it != draw_objects.end(); ++it)
    {
        (*it)->invalidate_draw(&draw_state);
    }
    menu_state_flow.invalidate_draw(&draw_state);
}

size_t GameState::get_screen_width() const
{
    return draw_state.screen_w;
}

size_t GameState::get_screen_height() const
{
    return draw_state.screen_h;
}

void GameState::set_screen_size(
    const size_t width,
    const size_t height)
{
    draw_state.screen_w = width;
    draw_state.screen_h = height;

    set_display(draw_state.display);
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
    // Update the sound volume based on menu state
    sound_manager.set_sound_gain(menu_state_flow.in_menu() ? 0.25 : 1.0);

    // Update the window offset if needed
    Vector2 diff_val = balloon.get_gondola().get_position() - draw_state.draw_offset;

    const double display_width = static_cast<double>(draw_state.screen_w);
    const double display_height = static_cast<double>(draw_state.screen_h);

    // Define the limit values
    const double lim_percent_x = 0.2;
    const double lim_percent_y = 0.4;

    const double lim_x_right = (1.0 - lim_percent_x) * display_width;
    const double lim_x_left = lim_percent_x * display_width;

    const double lim_y_top = lim_percent_y * display_height;
    const double lim_y_bot = (1.0 - lim_percent_y) * display_height;

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

    // Draw the menu if needed
    if (menu_state_flow.in_menu())
    {
        menu_state_flow.draw(&draw_state);
    }

    // Flip the screen
    al_flip_display();

    // Check for a button press to stop any music
    if (input_manager.get_key_rising_edge(ALLEGRO_KEY_M))
    {
        sound_manager.set_music_state(!sound_manager.get_music_state());
    }

    // Check for exit buttons
    if (input_manager.get_key_rising_edge(ALLEGRO_KEY_ESCAPE))
    {
        if (menu_state_flow.in_menu())
        {
            set_quit();
        }
        else
        {
            menu_state_flow.enter_menu();
        }
    }

    // Update sounds
    sound_manager.set_burner_state(balloon.get_envelope().get_burner_on() ? SoundManager::BurnerState::ON : SoundManager::BurnerState::OFF);
    sound_manager.set_valve_state(balloon.get_envelope().get_valve_open() ? SoundManager::ValveState::OPEN : SoundManager::ValveState::CLOSED);
    sound_manager.update_background();
}

void GameState::step(const double dt)
{
    // Determine the number of incremental steps to run
    const size_t num_steps = static_cast<size_t>(dt / world_state.time_step);

    // Update the autopilot if needed
    if (menu_state_flow.in_menu())
    {
        // Determine position state parameters
        const Vector2 gondola_pos = balloon.get_gondola().get_position();
        const double height_agl = terrain.elevation_at_x(gondola_pos.x) - gondola_pos.y;
        const double temp_percent = balloon.get_envelope().get_temp_ratio();

        // Perform bang-bang control
        if (height_agl < 290)
        {
            input_manager_autopilot.set_key_up(ALLEGRO_KEY_DOWN);

            if (temp_percent < 0.9)
            {
                input_manager_autopilot.set_key_down(ALLEGRO_KEY_UP);
            }
            else if (temp_percent > 0.95)
            {
                input_manager_autopilot.set_key_up(ALLEGRO_KEY_UP);
            }
        }
        else if (height_agl > 310)
        {
            if (temp_percent < 0.6)
            {
                input_manager_autopilot.set_key_up(ALLEGRO_KEY_DOWN);
            }
            else if (temp_percent > 0.8)
            {
                input_manager_autopilot.set_key_down(ALLEGRO_KEY_DOWN);
            }

            if (temp_percent < 0.5)
            {
                input_manager_autopilot.set_key_down(ALLEGRO_KEY_UP);
            }
            else if (temp_percent > 0.75)
            {
                input_manager_autopilot.set_key_up(ALLEGRO_KEY_UP);
            }
        }
    }
    else
    {
        world_state.input_manager = get_input_manager();
    }

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
