#ifndef GIO_INPUT_MANAGER_H
#define GIO_INPUT_MANAGER_H

#include <unordered_map>

/**
 * @brief Provides a basic input manager to maintain key state information
 */
class InputManager
{
public:
    /**
     * @brief constructs an input manager
     */
    InputManager();

    /**
     * @brief marks the given key as pressed
     * @param keycode the keycode to set
     */
    void set_key_down(const int keycode);

    /**
     * @brief marks the given key as released
     * @param keycode the keycode to set
     */
    void set_key_up(const int keycode);

    /**
     * @brief determines the state of a given key
     * @param keycode the keycode to check
     * @return true if the key state is pressed
     */
    bool get_key_status(const int keycode) const;

    /**
     * @brief determines if the current key has been pressed as a rising edge. Will reset
     * so subsequent calls for the same keycode will be false until the next key_down event
     * for the given key
     * @param keycode the keycode to set
     * @return true if the key is a new rising edge
    */
    bool get_key_rising_edge(const int keycode);

    /**
     * @brief determines if the up direction is pressed
     * @return true if up is pressed
     */
    bool get_dir_up() const;

    /**
     * @brief determines if the down direction is pressed
     * @return true if down is pressed
     */
    bool get_dir_down() const;

    /**
     * @brief determines if the left direction is pressed
     * @return true if left is pressed
     */
    bool get_dir_left() const;

    /**
     * @brief determines if the right direction is pressed
     * @return true if right is pressed
     */
    bool get_dir_right() const;

    /**
     * @brief determines if the enter action is pressed
     * @return true if the enter action is pressed
     */
    bool get_enter_pressed() const;

protected:
    std::unordered_map<int, bool> status_map;
    std::unordered_map<int, bool> rising_edge;
};

#endif // INPUT_MANAGER_H
