"""
Defines the drawing and physics requirements for the Balloon

Ian O'Rourke
2021
"""

import typing

import pygame

from game.objects import PhysicsObject, GameObject
from physics_state import GamePhysicsState


# Determines whether anchor points are drawn on the balloon
_DRAW_ANCHORS = False


class AeroObject(PhysicsObject):
    """
    Provides basic definitions for an aerodynamic object that has drag
    """

    def __init__(self, cd: float = 0.0, *args, **kwargs):
        """
        Initializes the aero object with a drag coefficient
        :param cd: the drag coefficient
        """
        super().__init__(*args, **kwargs)
        self.cd = cd

    def pre_step(self, state: GamePhysicsState) -> None:
        """
        Perform the physics computations for the rope
        :param state: the physics state to track information with
        """
        # Apply aerodynamic forces
        vm2 = self.velocity.magnitude_squared()

        if abs(vm2) > 1e-6:
            drag = -0.5 * self.cd * vm2 * self.velocity.normalize()
            self.add_force(drag)

        # Call the super-class parameters
        super().pre_step(state=state)


class Rope(GameObject):
    """
    Defines the physics object for a rope primitive
    """

    class Anchor:
        """
        Defines a rope anchor point
        """
        def __init__(
                self,
                point_func: typing.Callable[[], pygame.Vector2],
                obj: PhysicsObject):
            """
            Initializes the anchor point
            :param point_func: function to return the current location of an anchor point
            :param obj: the physics object to associated the forces with
            """
            self._point_func = point_func
            self.obj = obj

        def point(self) -> pygame.Vector2:
            """
            Provides the current anchor point location
            :return: the current anchor point
            """
            return self._point_func()

        def offset(self) -> pygame.Vector2:
            """
            Provides the offset from the anchor point to the physics object center of mass
            :return: the offset distance
            """
            return self.point() - self.obj.position

    def __init__(
            self,
            point_a: Anchor,
            point_b: Anchor,
            spring_k: float,
            limit_force: typing.Optional[float] = None):
        """
        Initializes a rope physics object
        :param point_a: the first anchor point
        :param point_b: the second anchor point
        :param spring_k: the spring constant ot use
        :param limit_force: the optional limit force to break the rope at
        """
        # Call the super class init
        super().__init__()

        # Saves the two point parameters
        self.point_a = point_a
        self.point_b = point_b

        # Saves physics parameters
        self.k = spring_k
        self.limit_force = limit_force

        # Saves the initial length values
        self._init_len = self.length()

        # Determines whether the rope is broken or not
        self.broken = False

    def length(self) -> float:
        """
        Provides the current length of the rope
        :return: the rope length
        """
        return (self.point_a.point() - self.point_b.point()).magnitude()

    def draw(self, surface: pygame.Surface, offset: pygame.Vector2) -> None:
        """
        Draws the rope
        :param surface: the surface to draw on
        :param offset: the offset value to use
        """
        # Draw any sub-parameters
        super().draw(surface=surface, offset=offset)

        # Skip drawing if broken
        if not self.broken:
            pygame.draw.line(
                surface=surface,
                color=(0, 0, 0),
                start_pos=self.point_a.point() - offset,
                end_pos=self.point_b.point() - offset)

    def pre_step(self, state: GamePhysicsState) -> None:
        """
        Perform the physics computations for the rope
        :param state: the physics state to track information with
        """
        # Add rope forces if not broken
        if not self.broken:
            # Determine the rope force and initial normalized direction
            rope_force = max((self.length() - self._init_len) * self.k, 0.0)
            rope_dir = (self.point_b.point() - self.point_a.point()).normalize()

            # Determine if the rope should break
            if self.limit_force is not None and rope_force > self.limit_force:
                self.broken = True
                rope_force = 0.0

            # Determine the rope force and direction
            rope_force_dir = rope_force * rope_dir

            # Apply the resulting forces on the respective objects
            self.point_a.obj.add_force(
                force=rope_force_dir,
                offset=self.point_a.offset())
            self.point_b.obj.add_force(
                force=rope_force_dir * -1.0,
                offset=self.point_b.offset())

        # Call the super pre_step function
        super().pre_step(state=state)


class Weight(AeroObject):
    """
    Provides the physics objects for a weight
    """

    def __init__(self):
        """
        Initializes the weight with default parameters
        """
        super().__init__(cd=0.1, mass=25, inertia=0.5)
        self.radius = 10.0

    def draw(self, surface: pygame.Surface, offset: pygame.Vector2) -> None:
        """
        Draws the weight
        :param surface: the surface to draw on
        :param offset: the offset value to use
        """
        # Draw any sub-parameters
        super().draw(surface=surface, offset=offset)

        pygame.draw.circle(
            surface=surface,
            color=(123, 79, 44),
            center=self.position - offset,
            radius=self.radius)

    def pre_step(self, state: GamePhysicsState) -> None:
        """
        Perform the physics computations for the rope
        :param state: the physics state to track information with
        """
        # Determine the elevation and surface normal of the terrain
        elev = state.terrain.elevation(x=self.position.x)
        norm = state.terrain.surface_normal(x=self.position.x)

        # Determine the point that we are using for elevation computation
        elev_point = pygame.Vector2(self.position.x, elev)

        # Setup the normal force values
        normal_force = pygame.Vector2(0.0, 0.0)

        # Determine the touch point that we will use for the weight object
        touch_point = self.position - norm * self.radius

        # Determine the distance to the ground (or that we are below the ground)
        ground_dist = (touch_point - elev_point).dot(norm)

        # If below the ground, apply the normal force
        if ground_dist < 0.0:
            normal_force += -ground_dist * state.terrain.spring_constant * norm
            normal_force += -min(0.0, norm.dot(self.velocity)) * state.terrain.damping_coefficient * norm

        # Add the force to the weight
        self.add_force(force=normal_force)

        # Call the super pre_step
        super().pre_step(state=state)


class Gondola(AeroObject):
    """
    Provides the physics object for the gondola
    """

    def __init__(self):
        """
        Initializes the default Gondola object
        """
        # Call the super init function
        super().__init__(cd=0.1, mass=50, inertia=3)

        # Initialize size parameters
        self.width = 40.0
        self.height = 30.0

    def top_left(self) -> pygame.Vector2:
        """
        Provides the top-left point
        :return: the top-left point
        """
        return self.position + pygame.Vector2(-self.width / 2.0, -self.height / 2.0).rotate_rad(self.rotation)

    def top_right(self) -> pygame.Vector2:
        """
        Provides the top-right point
        :return: the top-right point
        """
        return self.position + pygame.Vector2(self.width / 2.0, -self.height / 2.0).rotate_rad(self.rotation)

    def bottom_left(self) -> pygame.Vector2:
        """
        Provides the bottom-left point
        :return: the bottom-left point
        """
        return self.position + pygame.Vector2(-self.width / 2.0, self.height / 2.0).rotate_rad(self.rotation)

    def bottom_right(self) -> pygame.Vector2:
        """
        Provides the bottom-right point
        :return: the bottom-right point
        """
        return self.position + pygame.Vector2(self.width / 2.0, self.height / 2.0).rotate_rad(self.rotation)

    def draw(self, surface: pygame.Surface, offset: pygame.Vector2) -> None:
        """
        Draws the gondola
        :param surface: the surface to draw onto
        :param offset: the offset value to use
        """
        # Draw any sub-parameters
        super().draw(surface=surface, offset=offset)

        # Draw the base Gondola
        pygame.draw.polygon(
            surface=surface,
            color=(100, 100, 100),
            points=[
                self.top_right() - offset,
                self.top_left() - offset,
                self.bottom_left() - offset,
                self.bottom_right() - offset])

        # Draw the anchor points if requested
        if _DRAW_ANCHORS:
            for anchor in [self.top_left(), self.top_right()]:
                pygame.draw.circle(
                    surface=surface,
                    color=(0, 100, 0),
                    center=anchor - offset,
                    radius=5)
            for anchor in [self.bottom_left(), self.bottom_right()]:
                pygame.draw.circle(
                    surface=surface,
                    color=(100, 0, 0),
                    center=anchor - offset,
                    radius=5)

    def pre_step(self, state: GamePhysicsState) -> None:
        """
        Perform the physics computations for the rope
        :param state: the physics state to track information with
        """
        # Check for each corner point hitting the ground
        for point in [self.top_right(), self.top_left(), self.bottom_right(), self.bottom_left()]:
            # Determine the elevation point for the corner
            elev = state.terrain.elevation(x=point.x)

            # Determine the offset to the center and the resulting point velocity with the angular rotation
            point_offset = point - self.position
            point_vel = self.velocity + pygame.Vector2(-point_offset.y, point_offset.x) * self.omega

            # Obtain the surface normal
            norm = state.terrain.surface_normal(x=point.x)

            # Setup the normal and frictional forces
            normal_force = pygame.Vector2(0.0, 0.0)
            friction_force = pygame.Vector2(0.0, 0.0)

            # Calculate the normal force if needed
            if point.y > elev:
                normal_force += point.distance_to(pygame.Vector2(point.x, elev)) * state.terrain.spring_constant * norm
                normal_force += -min(0.0, norm.dot(point_vel)) * state.terrain.damping_coefficient * norm

            # Calculate the frictional force if needed
            if point.y > elev - 1:
                # Determine the along-terrain direction and magnitude velocity-wise
                along_terrain = norm.rotate(90.0)
                along_terrain *= -self.velocity.dot(along_terrain)

                # Determine static vs. kinematic frictional coefficient
                if abs(point_vel.dot(along_terrain)) > 1.0:
                    friction_coefficient = 0.15
                else:
                    friction_coefficient = 0.25

                # Set the frictional force
                friction_force = friction_coefficient * normal_force.magnitude() * along_terrain

            # Apply the forces
            self.add_force(force=normal_force, offset=point - self.position)
            self.add_force(force=friction_force, offset=point - self.position)

        # Add a rotational damping term
        self.moments += -self.rotation ** 2 * 0.1

        # Call the super pre-step
        super().pre_step(state=state)

    def step(self, state: GamePhysicsState) -> None:
        """
        Step of the physics objects
        :param state: the state to use for calculations
        """
        # Determine and limit the moment inputs
        moment_lim = 2000.0
        self.moments = min(max(-moment_lim, self.moments), moment_lim)

        # Call the super step
        super().step(state=state)


class Envelope(AeroObject):
    """
    Defines the Envelope physics object definition
    """

    def __init__(self):
        """
        Creates a default Envelope object
        """
        # Initialize the super class objects
        super().__init__(cd=0.4, mass=10, inertia=5)

        # Setup the size
        self.radius_min = 60.0
        self.radius_max = 100.0

        # Setup CD values
        self._cd_min = 0.4
        self._cd_max = 0.8

        # Setup lift and force parameters
        self.base_lift = 300
        self.max_lift = 1500
        self._temp_percent = 0.5

        self.lateral_force = 200

        self._color_cold = (200, 100, 100)
        self._color_hot = (200, 0, 0)

        # Allow for no temperature decay
        self.allow_temp_decay = False

        # Define the burner state
        self._burner_on = False
        self._valve_open = False

    def get_temp_percent(self) -> float:
        """
        Returns the temperature percent for the balloon
        :return: the temperature percent
        """
        return self._temp_percent

    @property
    def radius(self) -> float:
        """
        Provides the current radius value for the envelope
        :return: the current radius
        """
        return self._temp_interpolate(self.radius_min, self.radius_max)

    def _temp_interpolate(self, min_val: float, max_val: float) -> float:
        """
        Provides basic interpolation for temperature scaling
        :param min_val: the minimum, temp = 0.0, value
        :param max_val: the maximum, temp = 1.0, value
        :return: the interpolated value
        """
        return min_val * (1.0 - self._temp_percent) + max_val * self._temp_percent

    def _get_color(self) -> typing.List[int]:
        """
        Provides the appropriate envelope color for the current temperature value
        :return: the color specification
        """
        return [
            int(self._temp_interpolate(c, h))
            for c, h
            in zip(self._color_cold, self._color_hot)]

    def draw(self, surface: pygame.Surface, offset: pygame.Vector2) -> None:
        """
        Draws the envelope
        :param surface: the surface to draw onto
        :param offset: the offset value to use
        """
        # Draw any sub-parameters
        super().draw(surface=surface, offset=offset)

        # Draws the basic envelope
        pygame.draw.circle(
            surface=surface,
            color=self._get_color(),
            center=self.position - offset,
            radius=self.radius)

        # Draws anchor points if requested
        if _DRAW_ANCHORS:
            for anchor in [self.anchor_a(), self.anchor_b()]:
                pygame.draw.circle(
                    surface=surface,
                    color=(0, 100, 0),
                    center=anchor - offset,
                    radius=5)

    def anchor_a(self) -> pygame.Vector2:
        """
        Defines the first anchor point
        :return: first anchor point
        """
        return self.position + pygame.Vector2(-self.radius, 0.0).rotate(-30).rotate_rad(self.rotation)

    def anchor_b(self) -> pygame.Vector2:
        """
        Defines the second anchor point
        :return: second anchor point
        """
        return self.position + pygame.Vector2(self.radius, 0.0).rotate(30).rotate_rad(self.rotation)

    def burner_on(self) -> bool:
        """
        Determines if the burner is on
        :return: True if the burner is on
        """
        return self._burner_on

    def valve_open(self) -> bool:
        """
        Determines if the release valve is open
        :return: True if the valve is open
        """
        return self._valve_open

    def pre_step(self, state: GamePhysicsState) -> None:
        """
        Perform the physics computations for the rope
        :param state: the physics state to track information with
        """
        # Interpolate the drag coefficient
        self.cd = self._temp_interpolate(self._cd_min, self._cd_max)

        # Update the temperature scale
        if state.input_manager.up():
            self._temp_percent += 0.1 * state.dt
            self._burner_on = True
        else:
            self._burner_on = False

        if state.input_manager.down():
            self._temp_percent -= 0.1 * state.dt
            self._valve_open = True
        else:
            self._valve_open = False

        # Allow default attrition of the heat
        if self.allow_temp_decay:
            self._temp_percent -= 0.02 * self._temp_percent * state.dt

        # Constrain between 0 and 1
        self._temp_percent = min(max(0.0, self._temp_percent), 1.0)

        # Define the lift force
        lift_force = max(self._temp_interpolate(self.base_lift, self.max_lift), 0)

        # Add the lift force to the total force
        force = pygame.Vector2(0.0, -lift_force)

        # Add the lateral force based on left/right buttons
        if state.input_manager.left():
            force += pygame.Vector2(-self.lateral_force, 0.0)

        if state.input_manager.right():
            force += pygame.Vector2(self.lateral_force, 0.0)

        # Add the overall control force
        self.add_force(force)

        # Add a rotational-damping term
        self.moments += -self.rotation**2 * 0.1

        # Call the super pre-step function
        super().pre_step(state=state)


class Balloon(GameObject):
    """
    Defines an manages the entire balloon object
    """

    def __init__(self, x: float = 0.0, y: float = 0.0):
        """
        Initializes the Balloon with an optional x and y location
        :param x: the x location for the center of the gondola
        :param y: the y location for the center of the gondola
        """
        # Call the super init functions
        super().__init__()

        # Define the gondola and envelope
        self.envelope = Envelope()
        self.gondola = Gondola()

        self.gondola.position.update(x=x, y=y)
        self.envelope.position.update(x=x, y=y - 2 * self.envelope.radius)

        # Define standard rope parameters
        rope_k = 100.0
        rope_limit = 1500.0

        # Define the ropes for the envelope and gondola
        self.rope_1 = Rope(
            point_a=Rope.Anchor(
                point_func=lambda: self.envelope.anchor_a(),
                obj=self.envelope),
            point_b=Rope.Anchor(
                point_func=lambda: self.gondola.top_left(),
                obj=self.gondola),
            spring_k=rope_k,
            limit_force=rope_limit)
        self.rope_2 = Rope(
            point_a=Rope.Anchor(
                point_func=lambda: self.envelope.anchor_b(),
                obj=self.envelope),
            point_b=Rope.Anchor(
                point_func=lambda: self.gondola.top_right(),
                obj=self.gondola),
            spring_k=rope_k,
            limit_force=rope_limit)

        # Define the weights
        self.weight_1 = Weight()
        self.weight_2 = Weight()

        self.weight_1.position = self.gondola.bottom_left() + pygame.Vector2(0, 30.0)
        self.weight_2.position = self.gondola.bottom_right() + pygame.Vector2(0, 30.0)

        # Define the ropes for the weights
        self.rope_3 = Rope(
            point_a=Rope.Anchor(
                point_func=lambda: self.gondola.bottom_left(),
                obj=self.gondola),
            point_b=Rope.Anchor(
                point_func=lambda: self.weight_1.position,
                obj=self.weight_1),
            spring_k=rope_k,
            limit_force=rope_limit)
        self.rope_4 = Rope(
            point_a=Rope.Anchor(
                point_func=lambda: self.gondola.bottom_right(),
                obj=self.gondola),
            point_b=Rope.Anchor(
                point_func=lambda: self.weight_2.position,
                obj=self.weight_2),
            spring_k=rope_k,
            limit_force=rope_limit)

        # Define a list of all objects as part of the balloon
        balloon_objects = [
            self.rope_1,
            self.rope_2,
            self.rope_3,
            self.rope_4,
            self.envelope,
            self.gondola,
            self.weight_1,
            self.weight_2
        ]

        # Add the step and draw objects
        self.objects_step.extend(balloon_objects)
        self.objects_draw.extend(balloon_objects)

    def pre_step(self, state: GamePhysicsState) -> None:
        """
        Perform the physics computations for the rope
        :param state: the physics state to track information with
        """
        # Determine when to break ropes for the weights
        if state.input_manager.check_key(pygame.K_1):
            self.rope_3.broken = True
        if state.input_manager.check_key(pygame.K_2):
            self.rope_4.broken = True

        # Call the super pre-step function
        super().pre_step(state=state)
