#include "menu_state_flow.h"

#include <allegro5/allegro.h>

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <vector>
#include <string>
#include <fstream>

MenuStateFlow::MenuStateFlow()
{
    current_state = Location::MAIN;

    bmp_main = nullptr;
    bmp_help = nullptr;
    bmp_credits = nullptr;

    font_title = nullptr;
    font_text = nullptr;
}

bool MenuStateFlow::init(ALLEGRO_DISPLAY* display)
{
    // Define the font filename
    static const char* FONT_FILE_NAME = "fonts/tuffy.ttf";

    // Initialize the fonts
    font_title = al_load_ttf_font(FONT_FILE_NAME, 90, 0);
    font_text = al_load_ttf_font(FONT_FILE_NAME, 45, 0);
    font_credits = al_load_ttf_font(FONT_FILE_NAME, 25, 0);

    // Create the bitmaps
    return
        font_title != nullptr &&
        font_text != nullptr &&
        font_credits != nullptr &&
        init_main(display) &&
        init_help(display) &&
        init_credits(display);
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

    // Draw the title text
    al_draw_text(
        font_title,
        al_map_rgb(0, 0, 0),
        100.0,
        55.0,
        0,
        "Balloon Adventure");

    // Reset the draw target
    al_set_target_bitmap(prev);

    // Return the bitmap
    return bmp;
}

static bool init_bitmap_with_text(
    ALLEGRO_BITMAP* bitmap,
    ALLEGRO_FONT* font,
    const std::vector<std::string>& text_lines)
{
    // Check for null pointer
    if (bitmap == nullptr || font == nullptr)
    {
        return false;
    }

    // Setup for drawing text
    ALLEGRO_BITMAP* prev = al_get_target_bitmap();
    al_set_target_bitmap(bitmap);

    const int font_size = al_get_font_line_height(font);

    // Draw the text
    for (size_t i = 0; i < text_lines.size(); ++i)
    {
        al_draw_text(
            font,
            al_map_rgb(0, 0, 0),
            100.0,
            200.0 + i * 1.2 * font_size,
            0,
            text_lines[i].c_str());
    }

    // Reset the draw target
    al_set_target_bitmap(prev);

    // Return success
    return true;
}

bool MenuStateFlow::init_main(ALLEGRO_DISPLAY* display)
{
    // Setup the initial state
    bmp_main = create_default_bitmap(display);

    // Define the main lines
    const std::vector<std::string> main_lines = {
        "Press Enter to Start",
        "Press H for Help",
        "Press C for Credits",
        "Press Escape to Quit"
    };

    // Check that the bitmap was created correctly and finish drawing
    return init_bitmap_with_text(
        bmp_main,
        font_text,
        main_lines);
}

bool MenuStateFlow::init_help(ALLEGRO_DISPLAY* display)
{
    // Setup the initial state
    bmp_help = create_default_bitmap(display);

    // Define the main lines
    const std::vector<std::string> main_help = {
        "Use Arrow Keys to Move",
        "Press 1 to Release/Connect Left Weight",
        "Press 2 to Release/Connect Right Weight",
        "",
        "Press B to Return to Main Menu"
    };

    // Check that the bitmap was created correctly and finish drawing
    return init_bitmap_with_text(
        bmp_help,
        font_text,
        main_help);
}

bool MenuStateFlow::init_credits(ALLEGRO_DISPLAY* display)
{
    // Setup the initial state
    bmp_credits = create_default_bitmap(display);

    // Define the main lines
    std::vector<std::string> main_help;

    std::ifstream credits_input("music/credits.txt");

    std::string line;

    while (std::getline(credits_input, line))
    {
        main_help.push_back(line);
    }

    main_help.push_back("");
    main_help.push_back("Press B to Return to Main Menu");

    // Check that the bitmap was created correctly and finish drawing
    return init_bitmap_with_text(
        bmp_credits,
        font_credits,
        main_help);
}

void MenuStateFlow::draw(const DrawState* state)
{
    // Check for updated inputs
    switch (current_state)
    {
    case Location::MAIN:
        if (state->input_manager->get_key_rising_edge(ALLEGRO_KEY_ENTER))
        {
            current_state = Location::NONE;
        }
        else if (state->input_manager->get_key_rising_edge(ALLEGRO_KEY_C))
        {
            current_state = Location::CREDITS;
        }
        else if (state->input_manager->get_key_rising_edge(ALLEGRO_KEY_H))
        {
            current_state = Location::HELP;
        }
        break;
    case Location::HELP:
    case Location::CREDITS:
        if (state->input_manager->get_key_rising_edge(ALLEGRO_KEY_B))
        {
            current_state = Location::MAIN;
        }
        break;
    case Location::NONE:
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
    // Destroy the bitmaps
    try_destroy_bitmap(&bmp_main);
    try_destroy_bitmap(&bmp_help);
    try_destroy_bitmap(&bmp_credits);

    // Destroy the fonts
    al_destroy_font(font_title);
    font_title = nullptr;

    al_destroy_font(font_text);
    font_text = nullptr;

    al_destroy_font(font_credits);
    font_credits = nullptr;
}
