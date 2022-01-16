"""
Defines basic state objects for the game physics computations

Ian O'Rourke
2021
"""

from game.objects import StepState

from terrain import Terrain
from input_manager import InputManager


class GamePhysicsState(StepState):
    """
    Defines physics parameters for game state information
    """

    def __init__(self, terrain: Terrain, input_manager: InputManager):
        """
        Defines the basic game physics state
        :param terrain: the terrain model to use ground collisions
        :param input_manager: the input manager to use for input
        """
        # Initialize the super-state
        super().__init__()

        # Save the input parameters
        self.terrain = terrain
        self.input_manager = input_manager
