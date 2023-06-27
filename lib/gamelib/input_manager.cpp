#include <gamelib/input_manager.h>

#include <allegro5/keycodes.h>

InputManager::InputManager()
{
    // Empty Constructor
}

void InputManager::set_key_down(const int keycode)
{
    status_map[keycode].set_both(true);
}

void InputManager::set_key_up(const int keycode)
{
    status_map[keycode].set_both(false);
}

bool InputManager::get_key_status(const int keycode) const
{
    const auto it = status_map.find(keycode);
    if (it == status_map.end())
    {
        return false;
    }
    else
    {
        return it->second.press_status;
    }
}

bool InputManager::get_key_rising_edge(const int keycode)
{
    const bool val = status_map[keycode].rising_edge;
    if (val)
    {
        status_map[keycode].rising_edge = false;
    }
    return val;
}

bool InputManager::get_dir_up() const
{
    return get_key_status(ALLEGRO_KEY_UP) || get_key_status(ALLEGRO_KEY_W);
}

bool InputManager::get_dir_down() const
{
    return get_key_status(ALLEGRO_KEY_DOWN) || get_key_status(ALLEGRO_KEY_S);
}

bool InputManager::get_dir_left() const
{
    return get_key_status(ALLEGRO_KEY_LEFT) || get_key_status(ALLEGRO_KEY_A);
}

bool InputManager::get_dir_right() const
{
    return get_key_status(ALLEGRO_KEY_RIGHT) || get_key_status(ALLEGRO_KEY_D);
}

InputManager::KeyStatus::KeyStatus() :
    press_status{ false },
    rising_edge{ false }
{
    // Empty Constructor
}

void InputManager::KeyStatus::set_both(const bool val)
{
    press_status = val;
    rising_edge = val;
}
