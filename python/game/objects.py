"""
GameObject provides a basic physics base-object to work with

Ian O'Rourke
2021
"""

import pygame

import typing


class StepState:
    """
    Provides a basis for physics step parameters
    """
    def __init__(self):
        self.dt = 0.001


class StepObject:
    """
    Provides an interface to allow for a step-able object
    """

    def __init__(self):
        """
        Initializes the step object type
        """
        self.objects_step: typing.List[StepObject] = list()

    def pre_step(self, state: StepState) -> None:
        """
        Function to initialize any parameters pre-step, and called before each step
        :param state: the step state to use
        """
        for obj in self.objects_step:
            obj.pre_step(state=state)

    def step(self, state: StepState) -> None:
        """
        Steps the Physics of the object
        :param state: the step state to use
        """
        for obj in self.objects_step:
            obj.step(state=state)

    def post_step(self, state: StepState) -> None:
        """
        Any finalization parameters that run post-step of the object
        :param state: the step state to use
        """
        for obj in self.objects_step:
            obj.post_step(state=state)


class DrawObject:
    """
    Provides an instance to allow for a drawable object
    """

    def __init__(self):
        """
        Initializes the drawable object type
        """
        self.objects_draw: typing.List[DrawObject] = list()

    def draw(self, surface: pygame.Surface, offset: pygame.Vector2) -> None:
        """
        Draws the object on the given surface
        :param surface: the surface to draw the game object on
        :param offset: the offset value to use
        """
        for obj in self.objects_draw:
            obj.draw(surface=surface, offset=offset)


class GameObject(StepObject, DrawObject):
    """
    Provides a basic instance of a game object that has both a step and a draw function
    """
    def __init__(self):
        super(GameObject, self).__init__()
        super(StepObject, self).__init__()


class PhysicsObject(GameObject):
    """
    Provides basic parameters for a physics game object
    """

    def __init__(
            self,
            x: float = 0.0,
            y: float = 0.0,
            mass: float = 1.0,
            inertia: float = 1.0):
        """
        Initializes the game object with given parameters
        :param x: the starting X location
        :param y: the starting Y location
        :param mass: the mass of the object
        :param inertia: the inertia of the object
        """
        super().__init__()

        self.mass = mass
        self.inertia = inertia
        self.gravity = pygame.Vector2(x=0.0, y=10.0)

        self.position = pygame.Vector2(x=x, y=y)
        self.rotation = 0.0

        self.forces = pygame.Vector2(0.0, 0.0)
        self.moments = 0.0

        self.velocity = pygame.Vector2(0.0, 0.0)
        self.omega = 0.0

    def add_force(
            self,
            force: pygame.Vector2,
            offset: pygame.Vector2 = pygame.Vector2(),
            absolute: bool = True) -> None:
        """
        Adds a force to the current physics object
        :param force: the force values to add
        :param offset: optional offset to add the force location to
        :param absolute: determines if the force is absolute
        """
        if not absolute:
            force = force.rotate_rad(self.rotation)
        self.forces += force
        self.moments += offset.cross(force)

    def step(self, state: StepState) -> None:
        """
        Steps the Physics of the object
        :param state: the physics state to use
        """
        # Extract the time step
        dt = state.dt

        # Integrate translational motion
        self.velocity += self.forces / self.mass * dt + self.gravity * dt
        self.position += self.velocity * dt

        # Integrate rotational motion
        self.omega += self.moments / self.inertia * dt
        self.rotation = self.omega * dt

        # Reset the forces
        self.forces.update(0.0, 0.0)
        self.moments = 0.0

        # Update sub-objects
        super().step(state=state)
