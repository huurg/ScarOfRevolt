#ifndef HEADER_ANCHORWHEEL
#define HEADER_ANCHORWHEEL

#include <SDL.h>

class AnchorWheel {
    private:
        const double DEFAULT_WIDTH = 1;
        const double DEFAULT_HEIGHT = 1;
        const double DEFAULT_X = -8.4;
        const double DEFAULT_Y = -3.8;
        const double DEFAULT_ANCHOR_SIZE = 0.2;

        double scaling_factor_master;
        double scaling_factor_x;
        double scaling_factor_y;
        double position_modifier_x;
        double position_modifier_y;
        double anchor_size_modifier;
        SDL_Scancode* anchor_bindings;
        bool* down;
        bool* state;
        bool* previous_state;
        bool* up;
        void init();
        void deinit();
        //anchor_bindings //TODO Make keyboard-interactable
    public:
        AnchorWheel();
        ~AnchorWheel();
        void render();
        void step(int i, bool new_state);
        SDL_Scancode getScanCode(int i);
        bool isDown(int i) const;
        bool isUp(int i) const;
        bool isHeld(int i) const;
};

#endif // HEADER_ANCHORWHEEL
