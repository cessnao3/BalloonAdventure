"""
Provides Pygame sound management

Ian O'Rourke
2021
"""

import config
import pygame


class SoundManager:
    """
    Provide a basic sound manager for game sound
    """

    def __init__(self):
        """
        Initialize the game channels and sounds
        """
        # Define the burner channels
        self._ch_burner_start = pygame.mixer.Channel(1)
        self._ch_burner_end = pygame.mixer.Channel(2)

        # Define the music channel
        self._ch_music = pygame.mixer.Channel(3)
        self._ch_music.set_volume(0.5)

        # Define the background/wind channel
        self._ch_wind = pygame.mixer.Channel(4)
        self._ch_wind.set_volume(0.2)

        # Define the valve channel
        self._valve_channel = pygame.mixer.Channel(5)

        # Load in the sounds
        SOUNDS_DIR = config.RESOURCES_DIR / 'sounds'

        self._s_burner_start = pygame.mixer.Sound(SOUNDS_DIR / 'burner_start.wav')
        self._s_burner_end = pygame.mixer.Sound(SOUNDS_DIR / 'burner_end.wav')
        self._s_burner_loop = pygame.mixer.Sound(SOUNDS_DIR / 'burner_loop.wav')

        self._s_wind = pygame.mixer.Sound(SOUNDS_DIR / 'wind.wav')

        self._s_valve_open = pygame.mixer.Sound(SOUNDS_DIR / 'hiss.wav')

        # Define the music list
        MUSIC_DIR = config.RESOURCES_DIR / 'music'
        self._s_music_list = [
            pygame.mixer.Sound(MUSIC_DIR / 'gymnopedie_no_1.ogg'),
            pygame.mixer.Sound(MUSIC_DIR / 'gymnopedie_no_2.ogg'),
            pygame.mixer.Sound(MUSIC_DIR / 'gymnopedie_no_3.ogg')
            ]

        # Determine the last burner state
        self._last_burner_state = False

    def set_burner_state(self, state: bool) -> None:
        """
        Sets the burner state and modifies the sounds accordingly
        :param state: the burner state to use, True for on
        """
        # Perform checks if the last state does not equal the current state
        if self._last_burner_state != state:
            # Update the stored state
            self._last_burner_state = state

            # Check if we need to start or end the state
            if self._last_burner_state:
                self._ch_burner_end.fadeout(250)
                self._ch_burner_start.set_volume(1.0)
                self._ch_burner_start.play(self._s_burner_start)
            else:
                self._ch_burner_start.fadeout(250)
                self._ch_burner_end.set_volume(1.0)
                self._ch_burner_end.play(self._s_burner_end)

        # Start the loop if the burner is on but the init sound has finished playing
        if self._last_burner_state and not self._ch_burner_start.get_busy():
            self._ch_burner_start.play(self._s_burner_loop, loops=True)

    def set_valve_state(self, state: bool) -> None:
        """
        Sets the valve state for the envelope
        :param state: the valve state to use, True for open
        """
        if state and not self._valve_channel.get_busy():
            self._valve_channel.set_volume(1.0)
            self._valve_channel.play(self._s_valve_open, loops=True)
        elif not state and self._valve_channel.get_busy():
            self._valve_channel.fadeout(200)

    def update_background(self) -> None:
        """
        Checks the music/background noises to update if required
        """
        # Start the background wind if needed
        if not self._ch_wind.get_busy():
            self._ch_wind.play(self._s_wind, loops=True)

        # Loop through the music as desired
        if not self._ch_music.get_busy():
            new_sound = self._s_music_list.pop(0)
            self._s_music_list.append(new_sound)
            self._ch_music.play(new_sound)
