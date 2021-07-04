// Main Entry Point

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <game_state.h>

#include <iostream>

ALLEGRO_DISPLAY* create_display(
    GameState& state,
    bool fullscreen,
    ALLEGRO_TRANSFORM* transform)
{
    // Define the default scale factor
    double scale_factor = 0.0;

    // Apply options and adjust parameters for fullscreen or not
    if (fullscreen)
    {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
        scale_factor = 1.0;
    }
    else
    {
        al_set_new_display_flags(ALLEGRO_WINDOWED);
        scale_factor = 1.0;
        state.set_screen_size(
            1280,
            720);
    }

    // Create the display
    ALLEGRO_DISPLAY* display = al_create_display(
        static_cast<int>(state.get_screen_width() * scale_factor),
        static_cast<int>(state.get_screen_height() * scale_factor));

    // Hide the mouse cursor
    al_hide_mouse_cursor(display);

    // Get the new screen width / height
    const int new_screen_w = al_get_display_width(display);
    const int new_screen_h = al_get_display_height(display);

    // Set the new screen width and height
    state.set_screen_size(
        static_cast<size_t>(new_screen_w),
        static_cast<size_t>(new_screen_h));

    // Define the window title
    al_set_window_title(display, "Balloon Adventure");

    // Update the transformation
    al_identity_transform(transform);
    al_scale_transform(transform, scale_factor, scale_factor);
    //al_use_transform(transform);

    // Return the display
    return display;
}

int main()
{
    // Initialize the Allegro library
    if (!al_init())
    {
        std::cerr << "Unable to init Allegro library" << std::endl;
        return 1;
    }

    if (!al_install_keyboard())
    {
        std::cerr << "Unable to init Allegro keyboard addon" << std::endl;
        return 1;
    }

    if (!al_init_primitives_addon())
    {
        std::cerr << "Unable to init Allegro primitives addon" << std::endl;
        return 1;
    }

    if (!al_install_audio())
    {
        std::cerr << "Unable to init Allegro audio addon" << std::endl;
        return 1;
    }

    if (!al_init_acodec_addon())
    {
        std::cerr << "Unable to init Allegro audio codec addon" << std::endl;
        return 1;
    }

    if (!al_init_font_addon())
    {
        std::cerr << "Unable to init Allegro font addon" << std::endl;
        return 1;
    }

    if (!al_init_ttf_addon())
    {
        std::cerr << "Unable to init Allegro TTF addon" << std::endl;
        return 1;
    }

    // Setup Anti-Aliasing
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    // Create the main display
    bool is_in_fullscreen = false;

    ALLEGRO_TRANSFORM display_transform;
    ALLEGRO_DISPLAY* display = nullptr;

    // Create framerate and physics timers
    const double FRAME_PERIOD = 1.0 / 30.0;
    const double PHYSICS_PERIOD = 1.0 / 30.0;

    ALLEGRO_TIMER* frame_timer = al_create_timer(FRAME_PERIOD);
    ALLEGRO_TIMER* physics_timer = al_create_timer(PHYSICS_PERIOD);

    // Define a voice to use for playing audio
    ALLEGRO_VOICE* main_voice = al_create_voice(
        44100,
        ALLEGRO_AUDIO_DEPTH_INT16,
        ALLEGRO_CHANNEL_CONF_2);

    // Setup the event queue
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    // Define a scope for the game itself
    {
        // Define the game state
        GameState state;

        // Create the display
        display = create_display(
            state,
            is_in_fullscreen,
            &display_transform);
        al_register_event_source(
            event_queue,
            al_get_display_event_source(display));

        state.set_display(display);

        // Initialize the game state
        if (!state.init())
        {
            std::cerr << "Unable to initialize game state" << std::endl;
            state.set_quit();
        }

        // Assign the default mixer
        al_set_default_mixer(state.get_sound_manager()->get_mixer());
        al_attach_mixer_to_voice(state.get_sound_manager()->get_mixer(), main_voice);

        // Parameter to check for game events
        ALLEGRO_EVENT game_event;

        // Start Timers
        al_start_timer(frame_timer);
        al_start_timer(physics_timer);

        // Register event sources
        al_register_event_source(
            event_queue,
            al_get_timer_event_source(frame_timer));
        al_register_event_source(
            event_queue,
            al_get_timer_event_source(physics_timer));
        al_register_event_source(
            event_queue,
            al_get_keyboard_event_source());

        // Run the main game loop
        while (state.get_running())
        {
            // Wait for the next event
            al_wait_for_event(event_queue, &game_event);

            // Check each event type
            switch (game_event.type)
            {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                state.set_quit();
                break;
            case ALLEGRO_EVENT_TIMER:
                if (game_event.timer.source == physics_timer)
                {
                    // Run physics step
                    state.step(PHYSICS_PERIOD);
                }
                else if (game_event.timer.source == frame_timer)
                {
                    // Run frame step
                    state.draw();
                }
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                state.get_input_manager()->set_key_down(game_event.keyboard.keycode);
                if (state.get_input_manager()->get_key_rising_edge(ALLEGRO_KEY_F))
                {
                    // Stop Timers
                    al_pause_event_queue(event_queue, true);
                    al_flush_event_queue(event_queue);

                    // Remove the existing display as an event source
                    al_unregister_event_source(
                        event_queue,
                        al_get_display_event_source(display));
                    al_destroy_display(display);
                    display = nullptr;

                    // Switch the fullscreen state
                    is_in_fullscreen = !is_in_fullscreen;

                    // Create the new display and set parameters
                    display = create_display(
                        state,
                        is_in_fullscreen,
                        &display_transform);
                    state.set_display(display);

                    // Register the new display event source
                    al_register_event_source(
                        event_queue,
                        al_get_display_event_source(display));

                    // Restart Timers
                    al_pause_event_queue(event_queue, false);
                }
                break;
            case ALLEGRO_EVENT_KEY_UP:
                state.get_input_manager()->set_key_up(game_event.keyboard.keycode);
                break;
            default:
                break;
            }
        }
    }

    // Destory objects and null pointers
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(frame_timer);
    al_destroy_timer(physics_timer);
    al_destroy_voice(main_voice);

    display = nullptr;
    event_queue = nullptr;
    frame_timer = nullptr;
    physics_timer = nullptr;

    // Uninstall add-ons
    al_uninstall_audio();
    al_uninstall_keyboard();

    // Return success
    return 0;
}
