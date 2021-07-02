#include "menu_state_flow.h"

#include <allegro5/allegro.h>

MenuStateFlow::MenuStateFlow()
{
    current_state = Location::MAIN;

    bmp_main = nullptr;
    bmp_help = nullptr;
    bmp_credits = nullptr;
}

bool MenuStateFlow::init(ALLEGRO_DISPLAY* display)
{
    return init_main(display);

    /*
    return
        bmp_main != nullptr &&
        bmp_help != nullptr &&
        bmp_credits != nullptr;
        */
}

ALLEGRO_BITMAP* MenuStateFlow::create_default_bitmap(ALLEGRO_DISPLAY* display) const
{
    // Get the width and height of the menu
    const size_t width = al_get_display_width(display);
    const size_t height = al_get_display_height(display);

    // Create the bitmap
    ALLEGRO_BITMAP* bmp = al_create_bitmap(
        width,
        height);
    
    // Allow alpha blending
    al_set_blender(
        ALLEGRO_ADD,
        ALLEGRO_ALPHA,
        ALLEGRO_INVERSE_ALPHA);

    // Set the draw target
    ALLEGRO_BITMAP* prev = al_get_target_bitmap();
    al_set_target_bitmap(bmp);

    // Clear to transparent
    al_clear_to_color(al_map_rgba(255, 255, 255, 50));

    // Reset the draw target
    al_set_target_bitmap(prev);

    // Return the bitmap
    return bmp;
}

bool MenuStateFlow::init_main(ALLEGRO_DISPLAY* display)
{
    // Setup the initial state
    bmp_main = create_default_bitmap(display);

    // Return success
    return bmp_main != nullptr;
}

void MenuStateFlow::draw(const DrawState* state)
{
    // Check for updated inputs
    switch (current_state)
    {
    case Location::MAIN:
        if (state->input_manager->get_enter_pressed())
        {
            current_state = Location::NONE;
        }
        break;
    };

    ALLEGRO_BITMAP* selected_bitmap = nullptr;

    // Setup the bitmap based on the current state
    switch (current_state)
    {
    case Location::MAIN:
        selected_bitmap = bmp_main;
        break;
    case Location::HELP:
        selected_bitmap = bmp_help;
        break;
    case Location::CREDITS:
        selected_bitmap = bmp_credits;
        break;
    default:
        selected_bitmap = nullptr;
    }

    if (selected_bitmap != nullptr)
    {
        al_draw_bitmap(
            selected_bitmap,
            0.0f,
            0.0f,
            0);
    }
}

MenuStateFlow::Location MenuStateFlow::get_state() const
{
    return current_state;
}

static void try_destroy_bitmap(ALLEGRO_BITMAP** bmp)
{
    if (bmp != nullptr && *bmp != nullptr)
    {
        al_destroy_bitmap(*bmp);
        *bmp = nullptr;
    }
}

MenuStateFlow::~MenuStateFlow()
{
    try_destroy_bitmap(&bmp_main);
    try_destroy_bitmap(&bmp_help);
    try_destroy_bitmap(&bmp_credits);
}
