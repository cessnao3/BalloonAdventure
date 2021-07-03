#ifndef MENU_STATE_FLOW_H
#define MENU_STATE_FLOW_H

#include <gamelib/draw_object.h>

#include <allegro5/bitmap.h>
#include <allegro5/allegro_font.h>

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

    bool init(ALLEGRO_DISPLAY* display);

    virtual void draw(const DrawState* state);

    Location get_state() const;

    ~MenuStateFlow();

protected:
    ALLEGRO_BITMAP* create_default_bitmap(ALLEGRO_DISPLAY* display) const;

    bool init_main(ALLEGRO_DISPLAY* display);

    bool init_help(ALLEGRO_DISPLAY* display);

    bool init_credits(ALLEGRO_DISPLAY* display);

protected:
    Location current_state;

    ALLEGRO_BITMAP* bmp_main;
    ALLEGRO_BITMAP* bmp_help;
    ALLEGRO_BITMAP* bmp_credits;

    ALLEGRO_FONT* font_title;
    ALLEGRO_FONT* font_text;
    ALLEGRO_FONT* font_credits;
};

#endif // MENU_STATE_FLOW_H
