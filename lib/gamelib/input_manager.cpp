#include <gamelib/input_manager.h>

#include <allegro5/keycodes.h>

InputManager::InputManager()
{
    // Empty Constructor
}

void InputManager::set_key_down(const int keycode)
{
    status_map.insert_or_assign(keycode, true);
    rising_edge.insert_or_assign(keycode, true);
}

void InputManager::set_key_up(const int keycode)
{
    status_map.insert_or_assign(keycode, false);
    rising_edge.insert_or_assign(keycode, false);
}

bool InputManager::get_key_status(const int keycode) const
{
    const auto value = status_map.find(keycode);
    if (value == status_map.end())
    {
        return false;
    }
    else
    {
        return value->second;
    }
}

bool InputManager::get_key_rising_edge(const int keycode)
{
    const auto value = rising_edge.find(keycode);
    if (value == rising_edge.end())
    {
        return false;
    }
    else
    {
        const bool val = value->second;
        if (val)
        {
            rising_edge.insert_or_assign(keycode, false);
        }
        return val;
    }
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

bool InputManager::get_enter_pressed() const
{
    return get_key_status(ALLEGRO_KEY_ENTER);
}
