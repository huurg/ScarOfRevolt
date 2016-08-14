#include "AnchorWheel.h"

#include "RenderUtils.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <cmath>
#include <iostream>

using namespace std;

AnchorWheel::AnchorWheel() {
    init();
    scaling_factor_master = 1.0;
    scaling_factor_x = DEFAULT_WIDTH;
    scaling_factor_y = DEFAULT_HEIGHT;
    position_modifier_x = DEFAULT_X;
    position_modifier_y = DEFAULT_Y;
    anchor_size_modifier = DEFAULT_ANCHOR_SIZE;
}

AnchorWheel::~AnchorWheel() {
    deinit();
}

void AnchorWheel::render() {
    double dtheta = 2.0*M_PI/8.0;
    for(int i = 0; i < 8; i++) {
        double thistheta = -i*dtheta+M_PI/2.0;
        double this_x = scaling_factor_master*scaling_factor_x*cos(thistheta)+position_modifier_x;
        double this_y = scaling_factor_master*scaling_factor_y*sin(thistheta)+position_modifier_y;
        glColor3f(0.0f, 0.0f, 1.0f);
        if(state[i]) {
            glColor3f(1.0f, 0.5f, 0.0f);
        }
        if(down[i]) {
            glColor3f(0.0f, 1.0f, 1.0f);
        }
        if(up[i]){
            glColor3f(1.0f, 0.0f, 0.0f);
        }
        RenderUtils::renderCircle(anchor_size_modifier,this_x,this_y,20);
    }
    glColor3f(1.0f, 0.8f, 0.0f);
    if(state[8]) {
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    if(down[8]) {
        glColor3f(0.0f, 1.0f, 1.0f);
    }
    if(up[8]){
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    RenderUtils::renderCircle(2*anchor_size_modifier,position_modifier_x,position_modifier_y,20);
}

void AnchorWheel::init() {
    anchor_bindings = new SDL_Scancode[9];
    anchor_bindings[0] = SDL_SCANCODE_W;
    anchor_bindings[1] = SDL_SCANCODE_E;
    anchor_bindings[2] = SDL_SCANCODE_D;
    anchor_bindings[3] = SDL_SCANCODE_C;
    anchor_bindings[4] = SDL_SCANCODE_X;
    anchor_bindings[5] = SDL_SCANCODE_Z;
    anchor_bindings[6] = SDL_SCANCODE_A;
    anchor_bindings[7] = SDL_SCANCODE_Q;
    anchor_bindings[8] = SDL_SCANCODE_S;
    down = new bool[9];
    state = new bool[9];
    previous_state = new bool[9];
    up = new bool[9];
    for(int i = 0; i < 9; i++) {
        down[i] = state[i] = up[i] = previous_state[i] = false;
    }
}

void AnchorWheel::deinit() {
    delete [] anchor_bindings;
    delete [] down;
    delete [] state;
    delete [] up;
    delete [] previous_state;
}

void AnchorWheel::step(int i, bool new_state) {
    down[i] = false;
    up[i] = false;
    previous_state[i] = state[i];
    state[i] = new_state;
    if(!previous_state[i]&&state[i]) down[i] = true;
    else if(previous_state[i]&&!state[i]) up[i] = true;
}

SDL_Scancode AnchorWheel::getScanCode(int i) {
    return anchor_bindings[i];
}

bool AnchorWheel::isDown(int i) const {
    return down[i];
}

bool AnchorWheel::isUp(int i) const {
    return up[i];
}

bool AnchorWheel::isHeld(int i) const {
    return state[i];
}
