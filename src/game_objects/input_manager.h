#ifndef GIO_INPUT_MANAGER_H
#define GIO_INPUT_MANAGER_H

#include <unordered_map>
#include <allegro5/keycodes.h>

namespace gio
{

class InputManager
{
public:
    InputManager()
    {
        // Empty Constructor
    }

    void set_key_down(const int keycode)
    {
        status_map.insert_or_assign(keycode, true);
    }

    void set_key_up(const int keycode)
    {
        status_map.insert_or_assign(keycode, false);
    }

    bool get_key_status(const int keycode) const
    {
        auto value = status_map.find(keycode);
        if (value == status_map.end())
        {
            return false;
        }
        else
        {
            return value->second;
        }
    }

    bool get_dir_up() const
    {
        return get_key_status(ALLEGRO_KEY_UP) || get_key_status(ALLEGRO_KEY_W);
    }

    bool get_dir_down() const
    {
        return get_key_status(ALLEGRO_KEY_DOWN) || get_key_status(ALLEGRO_KEY_S);
    }

    bool get_dir_left() const
    {
        return get_key_status(ALLEGRO_KEY_LEFT) || get_key_status(ALLEGRO_KEY_A);
    }

    bool get_dir_right() const
    {
        return get_key_status(ALLEGRO_KEY_RIGHT) || get_key_status(ALLEGRO_KEY_D);
    }

    bool get_enter_pressed() const
    {
        return get_key_status(ALLEGRO_KEY_ENTER);
    }

protected:
    std::unordered_map<int, bool> status_map;
};

}

#endif // INPUT_MANAGER_H
