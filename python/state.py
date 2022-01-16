"""
The basic game state manager

Ian O'Rourke
2021
"""

import enum
import typing

import pygame

import config
from balloon import Balloon
from input_manager import InputManager
from game.objects import DrawObject, StepObject
from physics_state import GamePhysicsState
from sound_manager import SoundManager
from terrain import Terrain


class MenuEnum(enum.Enum):
    """
    Define a basic enumeration for the current menu state/location
    """
    NONE = 0
    IN_MENU = 1
    HELP = 2
    CREDITS = 3


class MenuState(DrawObject):
    """
    Class to maintain the menu state
    """

    def __init__(self):
        """
        Initializes menu parameters
        """
        # Initialize the super class
        super().__init__()

        # Initialize state parameters
        self.state = MenuEnum.IN_MENU

        # Define font and surface parameters
        self._surf_background: typing.Optional[pygame.Surface] = None

        # Define title parameters
        font = 'arial'
        self._font_tile = pygame.font.SysFont(font, 90)
        self._surf_title = self._font_tile.render('Balloon Adventure', True, (0, 0, 0))

        # Define instruction parameters
        self._font_instructions = pygame.font.SysFont(font, 45)

        # Define the font for credits
        self._font_credits = pygame.font.SysFont(font, 20)

        # Define the credit lines
        with (config.RESOURCES_DIR / 'credits.txt').open('r') as f:
            credit_lines = [line.strip() for line in f.read().strip().split('\n')]

        credit_lines.insert(0, '')
        credit_lines.insert(0, 'Game Created by Ian O\'Rourke')

        credit_lines.append('')
        credit_lines.append('Press B to Return to Main Menu')

        # Define the instruction surface list
        self._surf_inst_list = {
            MenuEnum.IN_MENU: [
                self._font_instructions.render('Press Enter to Start', True, (0, 0, 0)),
                self._font_instructions.render('Press H for Help', True, (0, 0, 0)),
                self._font_instructions.render('Press C for Credits', True, (0, 0, 0)),
                self._font_instructions.render('Press Escape to Quit', True, (0, 0, 0))
            ],
            MenuEnum.HELP: [
                self._font_instructions.render('Use Arrow Keys to Move', True, (0, 0, 0)),
                self._font_instructions.render('Press 1 to Release Left Weight', True, (0, 0, 0)),
                self._font_instructions.render('Press 2 to Release Right Weight', True, (0, 0, 0)),
                self._font_instructions.render('', True, (0, 0, 0)),
                self._font_instructions.render('Press B to Return to Main Menu', True, (0, 0, 0))
            ],
            MenuEnum.CREDITS: [
                self._font_credits.render(line, True, (0, 0, 0)) for line in credit_lines
            ]
        }

    def in_menu(self) -> bool:
        """
        Determines if we are in the menu
        :return: True if in the menu
        """
        return self.state != MenuEnum.NONE

    def update_keys(self, input_manager: InputManager) -> None:
        """
        Updates the menu for the given key presses
        """
        if self.state == MenuEnum.IN_MENU:
            if input_manager.check_key(pygame.K_RETURN) or input_manager.check_key(pygame.K_e):
                self.state = MenuEnum.NONE
            elif input_manager.check_key(pygame.K_c):
                self.state = MenuEnum.CREDITS
            elif input_manager.check_key(pygame.K_h):
                self.state = MenuEnum.HELP
        elif self.state in (MenuEnum.CREDITS, MenuEnum.HELP):
            if input_manager.check_key(pygame.K_b):
                self.state = MenuEnum.IN_MENU

    def draw(self, surface: pygame.Surface, offset: pygame.Vector2) -> None:
        """
        Draws the menu
        :param surface: the surface to draw on
        :param offset: the offset value to use
        """
        # Call super draw class
        super().draw(surface=surface, offset=offset)

        # Init the transparent surface if need
        if self._surf_background is not None:
            new_background_surf_needed = self._surf_background.get_width() != surface.get_width()
            new_background_surf_needed &= self._surf_background.get_height() != surface.get_height()
        else:
            new_background_surf_needed = True

        if new_background_surf_needed:
            self._surf_background = pygame.surface.Surface(
                (surface.get_width(), surface.get_height()),
                pygame.SRCALPHA)
            self._surf_background.fill((255, 255, 255, 50))

        # Draw the transparent background
        surface.blit(self._surf_background, (0, 0))

        # Draw the title
        surface.blit(self._surf_title, (100, 55))

        # Draw lines
        if self.state in (MenuEnum.IN_MENU, MenuEnum.HELP, MenuEnum.CREDITS):
            offset = 200
            for surf in self._surf_inst_list[self.state]:
                surface.blit(surf, (100, offset))
                offset += surf.get_height() + 0.3 * surf.get_height()


class GameState:
    """
    Defines an overall game state to use to manager the game
    """
    def __init__(self, screen: pygame.Surface):
        """
        Initializes the game state
        :param screen: the screen to use as a drawing surface
        """
        # Determine basic game parameters and save the input values
        self.running = True
        self.surface = screen
        self.input_manager = InputManager()
        self.sound_manager = SoundManager()

        # Define the autopilot input manager
        self.autopilot_manager = InputManager()

        # Define the menu state
        self.menu_state = MenuState()

        # Save the terrain values
        self.terrain = Terrain()

        # Create the balloon object
        self.balloon = Balloon(
            x=self.surface.get_width() / 2.0,
            y=self.surface.get_height() / 2.0)

        # Define a clock to use for tracking physics steps
        self.physics_clock = pygame.time.Clock()

        # Define the list of drawable objects
        self._drawable: typing.List[DrawObject] = [
            self.balloon,
            self.terrain
        ]

        # Define the list of step-able objects
        self._step: typing.List[StepObject] = [
            self.balloon
        ]

        # Define the basic physics state
        self.physics_state = GamePhysicsState(
            terrain=self.terrain,
            input_manager=self.autopilot_manager)

    def step(self) -> None:
        """
        Steps the physics values
        """
        # Determine the dt since the last step
        dt = self.physics_clock.tick()

        # Mark the time
        t = 0

        # Update the autopilot if needed
        if self.menu_state.in_menu():
            height = self.terrain.elevation(x=self.balloon.gondola.position.x) - self.balloon.gondola.position.y
            temp_percent = self.balloon.envelope.get_temp_percent()

            if height < 290:
                if temp_percent < 0.75:
                    self.autopilot_manager.key_down(key=pygame.K_UP)
                else:
                    self.autopilot_manager.key_up(key=pygame.K_UP)
            elif height > 310:
                if temp_percent > 0.6:
                    self.autopilot_manager.key_down(key=pygame.K_DOWN)
                else:
                    self.autopilot_manager.key_up(key=pygame.K_DOWN)

        # Run multiple physics time steps until we match the elapsed dt
        while t < dt:
            # Call pre-step for each object
            for obj in self._step:
                obj.pre_step(state=self.physics_state)

            # Call step for each object
            for obj in self._step:
                obj.step(state=self.physics_state)

            # Call post-step for each object
            for obj in self._step:
                obj.post_step(state=self.physics_state)

            # Increment the time
            t += self.physics_state.dt * 1000.0

    def draw(self) -> None:
        """
        Draw the scene to the surface
        """
        # Fill the scene
        self.surface.fill(
            (124, 199, 231),
            (0, 0, self.surface.get_width(), self.surface.get_height()))

        # Define the window offset
        window_offset = pygame.Vector2(0.0, 0.0)

        # Update the window offset if needed
        diff_val_x = self.balloon.gondola.position.x - window_offset.x
        diff_val_y = self.balloon.gondola.position.y - window_offset.y

        lim_x_right = 0.8 * self.surface.get_width()
        lim_x_left = 0.2 * self.surface.get_width()

        lim_y_top = 0.4 * self.surface.get_height()
        lim_y_bot = 0.8 * self.surface.get_height()

        if diff_val_x > lim_x_right:
            window_offset.x += diff_val_x - lim_x_right
        if diff_val_x < lim_x_left:
            window_offset.x += diff_val_x - lim_x_left

        if diff_val_y < lim_y_top:
            window_offset.y += diff_val_y - lim_y_top
        if diff_val_y > lim_y_bot:
            window_offset.y += diff_val_y - lim_y_bot

        # Draw each object
        for obj in self._drawable:
            obj.draw(surface=self.surface, offset=window_offset)

        # Update the sound manager
        self.sound_manager.update_background()

        # Update for menu key-presses
        if self.menu_state.in_menu():
            self.menu_state.draw(surface=self.surface, offset=window_offset)
            self.menu_state.update_keys(input_manager=self.input_manager)
            if not self.menu_state.in_menu():
                self.physics_state.input_manager = self.input_manager
                self.balloon.envelope.allow_temp_decay = True
        else:
            self.sound_manager.set_burner_state(state=self.balloon.envelope.burner_on())
            self.sound_manager.set_valve_state(state=self.balloon.envelope.valve_open())
