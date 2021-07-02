#ifndef MENU_STATE_FLOW_H
#define MENU_STATE_FLOW_H

#include <gamelib/draw_object.h>

#include <allegro5/bitmap.h>

class MenuStateFlow : DrawObject
{
public:
    enum class Location
    {
        NONE = 0,
        MAIN = 1,
        HELP = 2,
        CREDITS = 3
    };

public:
    MenuStateFlow();

    bool init(ALLEGRO_DISPLAY* display);

    virtual void draw(const DrawState* state);

    Location get_state() const;

    ~MenuStateFlow();

protected:
    ALLEGRO_BITMAP* create_default_bitmap(ALLEGRO_DISPLAY* display) const;

    bool init_main(ALLEGRO_DISPLAY* ALLEGRO_DISPLAY);

protected:
    Location current_state;

    ALLEGRO_BITMAP* bmp_main;
    ALLEGRO_BITMAP* bmp_help;
    ALLEGRO_BITMAP* bmp_credits;
};

/*
    class MenuState(DrawObject) :
    """
    Class to maintain the menu state
    """

    def __init__(self) :
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
    with(config.RESOURCES_DIR / 'credits.txt').open('r') as f :
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
    ] ,
    MenuEnum.HELP: [
        self._font_instructions.render('Use Arrow Keys to Move', True, (0, 0, 0)),
        self._font_instructions.render('Press 1 to Release Left Weight', True, (0, 0, 0)),
        self._font_instructions.render('Press 2 to Release Left Weight', True, (0, 0, 0)),
        self._font_instructions.render('', True, (0, 0, 0)),
        self._font_instructions.render('Press B to Return to Main Menu', True, (0, 0, 0))
    ] ,
    MenuEnum.CREDITS: [
        self._font_credits.render(line, True, (0, 0, 0)) for line in credit_lines
    ]
}

def in_menu(self) -> bool:
        """
            Determines if we are in the menu
            : return : True if in the menu
            """
            return self.state != MenuEnum.NONE

            def update_keys(self, input_manager: InputManager)->None :
            """
            Updates the menu for the given key presses
            """
            if self.state == MenuEnum.IN_MENU :
                if input_manager.check_key(pygame.K_RETURN) or input_manager.check_key(pygame.K_e) :
                    self.state = MenuEnum.NONE
                    elif input_manager.check_key(pygame.K_c) :
                    self.state = MenuEnum.CREDITS
                    elif input_manager.check_key(pygame.K_h) :
                    self.state = MenuEnum.HELP
                    elif self.state in(MenuEnum.CREDITS, MenuEnum.HELP) :
                    if input_manager.check_key(pygame.K_b) :
                        self.state = MenuEnum.IN_MENU

                        def draw(self, surface: pygame.Surface, offset : pygame.Vector2)->None :
                        """
                        Draws the menu
                        : param surface : the surface to draw on
                        : param offset : the offset value to use
                        """
                        # Call super draw class
                        super().draw(surface = surface, offset = offset)

                        # Init the transparent surface if need
                        if self._surf_background is not None:
        new_background_surf_needed = self._surf_background.get_width() != surface.get_width()
            new_background_surf_needed &= self._surf_background.get_height() != surface.get_height()
                        else:
        new_background_surf_needed = True

            if new_background_surf_needed :
                self._surf_background = pygame.surface.Surface(
                    (surface.get_width(), surface.get_height()),
                    pygame.SRCALPHA)
                self._surf_background.fill((255, 255, 255, 50))

                # Draw the transparent background
                surface.blit(self._surf_background, (0, 0))

                # Draw the title
                surface.blit(self._surf_title, (100, 55))

                # Draw lines
                if self.state in(MenuEnum.IN_MENU, MenuEnum.HELP, MenuEnum.CREDITS) :
                    offset = 200
                    for surf in self._surf_inst_list[self.state] :
                        surface.blit(surf, (100, offset))
                        offset += surf.get_height() + 0.3 * surf.get_height()
                        */

#endif // MENU_STATE_FLOW_H
