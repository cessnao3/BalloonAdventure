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

    bitmap_stored = nullptr;

    font_title = nullptr;
    font_text = nullptr;
    font_credits = nullptr;
}

bool MenuStateFlow::init(const DrawState* state)
{
    // Define the font filename
    static const char* FONT_FILE_NAME = "fonts/tuffy.ttf";

    // Initialize the fonts
    if (font_title == nullptr)
    {
        font_title = al_load_ttf_font(FONT_FILE_NAME, 90, 0);
    }

    if (font_text == nullptr)
    {
        font_text = al_load_ttf_font(FONT_FILE_NAME, 45, 0);
    }

    if (font_credits == nullptr)
    {
        font_credits = al_load_ttf_font(FONT_FILE_NAME, 25, 0);
    }

    // Create the bitmaps
    return
        font_title != nullptr &&
        font_text != nullptr &&
        font_credits != nullptr;
}

bool MenuStateFlow::draw_default_bitmap(
    const DrawState* state,
    ALLEGRO_FONT* font,
    const std::vector<std::string>& lines)
{
    // Check for null pointer
    if (font == nullptr)
    {
        return false;
    }

    // Create the bitmap if needed
    if (bitmap_stored == nullptr)
    {
        bitmap_stored = al_create_bitmap(
            static_cast<int>(state->screen_w),
            static_cast<int>(state->screen_h));
    }

    if (bitmap_stored == nullptr)
    {
        return false;
    }

    // Set the draw target
    ALLEGRO_BITMAP* prev = al_get_target_bitmap();
    al_set_target_bitmap(bitmap_stored);

    // Allow alpha blending
    al_set_blender(
        ALLEGRO_ADD,
        ALLEGRO_ALPHA,
        ALLEGRO_INVERSE_ALPHA);

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

    // Draw the text
    const int font_size = al_get_font_line_height(font);

    for (size_t i = 0; i < lines.size(); ++i)
    {
        al_draw_text(
            font,
            al_map_rgb(0, 0, 0),
            100.0,
            200.0 + i * 1.2 * font_size,
            0,
            lines[i].c_str());
    }

    // Reset the draw target
    al_set_target_bitmap(prev);

    // Return success
    return true;
}

bool MenuStateFlow::draw_main(const DrawState* state)
{
    // Define the main lines
    const std::vector<std::string> main_lines = {
        "Press Enter to Start",
        "Press H for Help",
        "Press C for Credits",
        "Press Escape to Quit"
    };

    // Draw the bitmap
    return draw_default_bitmap(
        state,
        font_text,
        main_lines);
}

bool MenuStateFlow::draw_help(const DrawState* state)
{
    // Define the main lines
    const std::vector<std::string> main_help = {
        "Use Arrow Keys to Move",
        "Press 1 to Release/Connect Left Weight",
        "Press 2 to Release/Connect Right Weight",
        "Press F to Toggle Windowed / Fullscreen",
        "Press M to Toggle Music",
        "",
        "Press B to Return to Main Menu"
    };

    // Draw the bitmap
    return draw_default_bitmap(
        state,
        font_text,
        main_help);
}

bool MenuStateFlow::draw_credits(const DrawState* state)
{
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
    return draw_default_bitmap(
        state,
        font_credits,
        main_help);
}

void MenuStateFlow::draw(const DrawState* state)
{
    // Define the new state
    Location new_state = current_state;

    // Check for updated inputs
    switch (current_state)
    {
    case Location::MAIN:
        if (state->input_manager->get_key_rising_edge(ALLEGRO_KEY_ENTER))
        {
            new_state = Location::NONE;
        }
        else if (state->input_manager->get_key_rising_edge(ALLEGRO_KEY_C))
        {
            new_state = Location::CREDITS;
        }
        else if (state->input_manager->get_key_rising_edge(ALLEGRO_KEY_H))
        {
            new_state = Location::HELP;
        }
        break;
    case Location::HELP:
    case Location::CREDITS:
        if (state->input_manager->get_key_rising_edge(ALLEGRO_KEY_B))
        {
            new_state = Location::MAIN;
        }
        break;
    case Location::NONE:
        break;
    };

    // Setup the bitmap based on the current state
    if (bitmap_stored == nullptr || new_state != current_state)
    {
        bool success = true;

        switch (new_state)
        {
        case Location::MAIN:
            success = draw_main(state);
            break;
        case Location::HELP:
            success = draw_help(state);
            break;
        case Location::CREDITS:
            success = draw_credits(state);
            break;
        default:
            break;
        }

        if (!success)
        {
            throw std::runtime_error("unable to draw menu screen");
        }

        current_state = new_state;
    }

    // Determine the selected bitmap
    ALLEGRO_BITMAP* selected_bitmap = nullptr;

    switch (current_state)
    {
    case Location::MAIN:
    case Location::HELP:
    case Location::CREDITS:
        selected_bitmap = bitmap_stored;
        break;
    default:
        selected_bitmap = nullptr;
    }

    // Draw if provided
    if (selected_bitmap != nullptr)
    {
        al_draw_bitmap(
            selected_bitmap,
            0.0f,
            0.0f,
            0);
    }
}

void MenuStateFlow::invalidate_draw(const DrawState* state)
{
    if (bitmap_stored != nullptr)
    {
        al_destroy_bitmap(bitmap_stored);
        bitmap_stored = nullptr;
    }

    init(state);
}

MenuStateFlow::Location MenuStateFlow::get_state() const
{
    return current_state;
}

bool MenuStateFlow::in_menu() const
{
    return current_state != Location::NONE;
}

void MenuStateFlow::enter_menu()
{
    current_state = Location::MAIN;
}

MenuStateFlow::~MenuStateFlow()
{
    // Destroy bitmaps
    if (bitmap_stored != nullptr)
    {
        al_destroy_bitmap(bitmap_stored);
        bitmap_stored = nullptr;
    }

    // Destroy the fonts
    if (font_title != nullptr)
    {
        al_destroy_font(font_title);
        font_title = nullptr;
    }

    if (font_text != nullptr)
    {
        al_destroy_font(font_text);
        font_text = nullptr;
    }

    if (font_credits != nullptr)
    {
        al_destroy_font(font_credits);
        font_credits = nullptr;
    }
}
