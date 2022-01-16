"""
Provides basic event ID values for user events

Ian O'Rourke
2021
"""

import pygame

# Event for when it is time to draw the scene
TIMER_DRAW = pygame.USEREVENT

# Event for when it is time to run a physics time step
TIMER_STEP = pygame.USEREVENT + 1
