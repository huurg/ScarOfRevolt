#include "SingleKeyBinding.h"

#include <SDL.h>

SingleKeyBinding::SingleKeyBinding(SDL_Scancode inBinding) {
    binding = inBinding;
    up = down = state = previous_state = false;
}

void SingleKeyBinding::step(bool new_state) {
    previous_state = state;
    state = new_state;
    up = down = false;
    if(!previous_state&&state) {
        down = true;
    }
    if(previous_state&&!state) {
        up = true;
    }
}

SDL_Scancode SingleKeyBinding::getBinding() const {
    return binding;
}

bool SingleKeyBinding::isDown() const {
    return down;
}

bool SingleKeyBinding::isUp() const {
    return up;
}

bool SingleKeyBinding::isHeld() const {
    return state;
}
