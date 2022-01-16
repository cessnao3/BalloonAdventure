"""
Balloon Adventure Game
Main Script

Ian O'Rourke
2021
"""

# import os
# os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = "hide"


import pygame

from game import events
from state import GameState

import config


def main() -> None:
    """
    Defines the main function
    """
    # initialize pygame
    pygame.init()

    # Load and set the window icon and caption
    icon = pygame.image.load(config.RESOURCES_DIR / 'balloon_icon.png')
    pygame.display.set_icon(icon)
    pygame.display.set_caption('Balloon Adventure')

    # Define the main screen
    screen = pygame.display.set_mode((640 * 2, int(480 * 1.5)))

    # Setup the game state
    state = GameState(screen=screen)

    # Define the basic intervals
    intervals = {
        events.TIMER_DRAW: 30,
        events.TIMER_STEP: 30
    }

    # Define the user-event timers
    max_frame_rate = 0
    for event_id, frame_rate in intervals.items():
        pygame.time.set_timer(
            event=event_id,
            millis=1000 // frame_rate)
        max_frame_rate = max(max_frame_rate, frame_rate)

    # Define the game clock
    game_clock = pygame.time.Clock()

    # Loop while needed
    while state.running:
        # Loop the game
        loop(state=state)

        # Pause the sim for the maximum frame rate value
        game_clock.tick(max_frame_rate)

    # Quit at end
    pygame.quit()


def loop(state: GameState) -> None:
    """
    Called as the primary game loop
    :param state: the game state to use in the loop
    """
    # Iterate over each event
    for event in pygame.event.get():
        # Check for each specific event type
        if event.type == pygame.QUIT:
            state.running = False
        elif event.type == events.TIMER_DRAW:
            state.draw()
            pygame.display.flip()
        elif event.type == events.TIMER_STEP:
            state.step()
        elif event.type == pygame.KEYDOWN:
            state.input_manager.key_down(key=event.key)
            if state.input_manager.quit():
                state.running = False
        elif event.type == pygame.KEYUP:
            state.input_manager.key_up(key=event.key)


if __name__ == '__main__':
    main()
