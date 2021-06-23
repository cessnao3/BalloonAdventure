// Main Entry Point

#include <allegro5/allegro.h>

#include <game_state.h>

int main()
{
    // Initialize the Allegro library
    if (!al_init())
    {
        return 1;
    }

    // Create the main display
    ALLEGRO_DISPLAY* display = al_create_display(800, 600);
    al_set_window_title(display, "Balloon Adventure");

    // Create framerate and physics timers
    ALLEGRO_TIMER* frame_timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_TIMER* physics_timer = al_create_timer(1.0 / 60.0);

    // Setup the event queue
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    // Add the display and timer events to the queue
    al_register_event_source(
        event_queue,
        al_get_display_event_source(display));
    al_register_event_source(
        event_queue,
        al_get_timer_event_source(frame_timer));
    al_register_event_source(
        event_queue,
        al_get_timer_event_source(physics_timer));

    // Start Timers
    al_start_timer(frame_timer);
    al_start_timer(physics_timer);

    // Determine if the game should be running
    GameState state;

    // Parameter to check for game events
    ALLEGRO_EVENT game_event;

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
            }
            else if (game_event.timer.source == frame_timer)
            {
                // Run frame step
            }
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            break;
        case ALLEGRO_EVENT_KEY_UP:
            break;
        default:
            break;
        }
    }

    // Destory objects and null pointers
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(frame_timer);
    al_destroy_timer(physics_timer);

    display = nullptr;
    event_queue = nullptr;
    frame_timer = nullptr;
    physics_timer = nullptr;

    // Return success
    return 0;
}
