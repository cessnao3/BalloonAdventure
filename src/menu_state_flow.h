#ifndef MENU_STATE_FLOW_H
#define MENU_STATE_FLOW_H

#include <gamelib/draw_object.h>

#include <allegro5/bitmap.h>
#include <allegro5/allegro_font.h>

#include <vector>
#include <string>

class MenuStateFlow : DrawObject
{
public:
    enum class Location
    {
        NONE = 0,
        MAIN = 1,
        HELP = 2,
        CREDITS = 3
    };

public:
    MenuStateFlow();

    bool init(const DrawState* state);

    virtual void draw(const DrawState* state);

    virtual void invalidate_draw(const DrawState* state) override;

    Location get_state() const;

    bool in_menu() const;

    void enter_menu();

    ~MenuStateFlow();

protected:
    bool draw_default_bitmap(
        const DrawState* state,
        ALLEGRO_FONT* font,
        const std::vector<std::string>& lines);

    bool draw_main(const DrawState* state);

    bool draw_help(const DrawState* state);

    bool draw_credits(const DrawState* state);

protected:
    Location current_state;

    ALLEGRO_BITMAP* bitmap_stored;

    ALLEGRO_FONT* font_title;
    ALLEGRO_FONT* font_text;
    ALLEGRO_FONT* font_credits;
};

#endif // MENU_STATE_FLOW_H
