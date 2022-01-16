"""
Defines an input manager to maintain key press inputs

Ian O'Rourke
2021
"""

import typing

import pygame


class InputManager:
    """
    Allows for ease of accessing key press events
    """

    def __init__(self):
        """
        Initializes the input manager
        """
        self._keys: typing.Dict[int, bool] = dict()

    def key_up(self, key: int) -> None:
        """
        Marks a particular key as not pressed
        :param key: the key code to modify
        """
        self._keys[key] = False

    def key_down(self, key: int):
        """
        Marks a particular key as pressed
        :param key: the key code to modify
        """
        self._keys[key] = True

    def check_key(self, key: int) -> bool:
        """
        Checks if a key is pressed
        :param key: the key code to check
        :return: True if pressed; otherwise False
        """
        return self._keys.get(key, False)

    def left(self) -> bool:
        """
        Determines if the left directional key is pressed
        :return: True if pressed
        """
        return self.check_key(key=pygame.K_LEFT) or self.check_key(key=pygame.K_a)

    def right(self) -> bool:
        """
        Determines if the right directional key is pressed
        :return: True if pressed
        """
        return self.check_key(key=pygame.K_RIGHT) or self.check_key(key=pygame.K_d)

    def up(self) -> bool:
        """
        Determines if the up directional key is pressed
        :return: True if pressed
        """
        return self.check_key(key=pygame.K_UP) or self.check_key(key=pygame.K_w)

    def down(self) -> bool:
        """
        Determines if the down directional key is pressed
        :return: True if pressed
        """
        return self.check_key(key=pygame.K_DOWN) or self.check_key(key=pygame.K_s)

    def quit(self) -> bool:
        """
        Determines if an exit key is pressed
        :return: True if pressed
        """
        return self.check_key(key=pygame.K_ESCAPE)
