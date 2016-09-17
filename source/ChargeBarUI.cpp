#include "ChargeBarUI.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <cmath>
#include <iostream>

ChargeBarUI::ChargeBarUI(double inMaxCharge, double inMaxConc) {
    height = DEFAULT_HEIGHT;
    width = DEFAULT_WIDTH;
    position_x = DEFAULT_POSITION_X;
    position_y = DEFAULT_POSITION_Y;
    percentage_charged = charge = percentage_conc = conc = 0.0;
    max_charge = inMaxCharge;
    max_conc = inMaxConc;
}

void ChargeBarUI::render() {
    percentage_charged = charge/max_charge;
    percentage_conc = conc/max_conc;
    glBegin(GL_QUADS);
        glColor3d(0.1,0.1,0.1);
        glVertex2d(position_x-width/2.0, position_y+height/2.0);
        glVertex2d(position_x+width/2.0, position_y+height/2.0);
        glVertex2d(position_x+width/2.0, position_y-height/2.0);
        glVertex2d(position_x-width/2.0, position_y-height/2.0);

        glColor3d(1.0,0.0,0.0);
        glVertex2d(position_x-width/2.0, position_y+height/2.0);
        glVertex2d(position_x-width/2.0+width*percentage_charged, position_y+height/2.0);
        glVertex2d(position_x-width/2.0+width*percentage_charged, position_y-height/2.0);
        glVertex2d(position_x-width/2.0, position_y-height/2.0);

        glColor3d(0.0,1.0,0.0);
        glVertex2d(position_x-width/2.0, position_y-height/4.0);
        glVertex2d(position_x-width/2.0+width*percentage_conc, position_y-height/4.0);
        glVertex2d(position_x-width/2.0+width*percentage_conc, position_y-height/2.0);
        glVertex2d(position_x-width/2.0, position_y-height/2.0);
    glEnd();
}

void ChargeBarUI::setCharge(double inCharge) {
    charge = inCharge;
}

void ChargeBarUI::setConcentration(double inConc) {
    conc = inConc;
}
