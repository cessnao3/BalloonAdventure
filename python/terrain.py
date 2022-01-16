"""
Terrain model for the system

Ian O'Rourke
2021
"""
import math

import pygame

from game.objects import DrawObject


class Terrain(DrawObject):
    """
    Defines the overall terrain model to use for physics computations
    """

    # Define whether to draw surface normals
    _DRAW_NORMALS = False

    def __init__(self):
        """
        Initializes the terrain
        """
        # Init the super items
        super().__init__()

        # Define the base height and surface normal
        self.base_height = 650.0

        # Define physics parameters for the terrain
        self.spring_constant = 500.0
        self.damping_coefficient = 250.0
        self.friction_damping = 100.0

        # Define terrain values
        self.base_amplitude = 20.0
        self.base_frequency = 0.01

    def elevation(self, x: float) -> float:
        """
        Determine the elevation at a particular point
        :param x: the x location to check
        :return: the corresponding elevation value
        """
        return self.base_height + self.base_amplitude * math.sin(self.base_frequency * x)

    def surface_normal(self, x: float) -> pygame.Vector2:
        """
        Determine the surface normal at a particular point
        :param x: the x location to check
        :return: the corresponding surface normal value
        """
        return pygame.Vector2(
            self.base_amplitude * self.base_frequency * math.cos(self.base_frequency * x),
            -1).normalize()

    def draw(self, surface: pygame.Surface, offset: pygame.Vector2) -> None:
        """
        Draws the terrain
        :param surface: the surface to draw on
        :param offset: the offset vector
        """
        # Determine the lines to use in drawing the terrain
        terrain_lines = [
            surface.get_bounding_rect().bottomleft
        ]
        terrain_lines.extend([
            pygame.Vector2(x + offset.x, self.elevation(x=x + offset.x)) - offset
            for x in range(surface.get_width())])
        terrain_lines.append(surface.get_bounding_rect().bottomright)

        # Draw the terrain polygon
        pygame.draw.polygon(
            surface=surface,
            color=(50, 150, 75),
            points=terrain_lines)

        # Draw the surface normals
        if self._DRAW_NORMALS:
            for x in range(0, surface.get_width(), 10):
                point = pygame.Vector2(x, self.elevation(x=x))
                norm = self.surface_normal(x=x)
                pygame.draw.line(
                    surface=surface,
                    start_pos=point,
                    end_pos=point + norm * 10,
                    color=(0, 0, 0))
