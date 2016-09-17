#ifndef SINGLE_KEY_BINDING_HEADER
#define SINGLE_KEY_BINDING_HEADER

#include <SDL.h>

class SingleKeyBinding {
private:
    bool state;
    bool previous_state;
    bool up;
    bool down;
    SDL_Scancode binding;
public:
    SingleKeyBinding(SDL_Scancode inBinding);
    void step(bool new_state);
    SDL_Scancode getBinding() const;
    bool isDown() const;
    bool isUp() const;
    bool isHeld() const;
};

#endif // SINGLE_KEY_BINDING_HEADER
