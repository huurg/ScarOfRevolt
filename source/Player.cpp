#include "Player.h"
#include "Circle.h"

#include <iostream>
using namespace std;

Player::Player() {
    can_charge = true;
    charge = 0.0;
    charge_max = DEFAULT_CHARGE_MAX;
    charge_rate = DEFAULT_CHARGE_RATE;
    chargeState = CHARGE_STATE_NONE;

    can_concentrate = true;
    concentration = 0.0;
    concentration_max = DEFAULT_CONCENTRATION_MAX;
    concentration_rate = DEFAULT_CONCENTRATION_RATE;
    concentrationState = CONCENTRATION_STATE_NONE;

    up = down = held = false;
    player_circle = NULL;
}

void Player::setPlayerCircle(Circle* inC) {
    player_circle = inC;
}

void Player::start_charge() {
    if(can_charge) {
        chargeState = CHARGE_STATE_CHARGING;
    }
    if(can_concentrate) {
        concentrationState = CONCENTRATION_STATE_CONCENTRATING;
    }
}

void Player::end_charge() {
    Circle* thisC = new Circle(player_circle->getRX()+3*player_circle->R*player_circle->v->x()/player_circle->v->R(), player_circle->getRY()+3*player_circle->R*player_circle->v->y()/player_circle->v->R(), player_circle->v->x()*(1.0+(20*charge/charge_max/player_circle->v->R())), player_circle->v->y()*(1.0+(20*charge/charge_max/player_circle->v->R())), 0.1, 1);
    thisC->friction  = 0.0;
    bool success = createEntity(thisC);
    if(!success) delete thisC;
    charge = 0.0;
    concentration = 0.0;
    chargeState = CHARGE_STATE_NONE;
    concentrationState = CONCENTRATION_STATE_NONE;
}

void Player::frameListen(bool inUp, bool inDown, bool inHeld) {
    up = inUp;
    down = inDown;
    held = inHeld;
}

void Player::step() {
    if(down) start_charge();
    else if(up) end_charge();

    switch(chargeState) {
        case CHARGE_STATE_NONE:
            break;
        case CHARGE_STATE_CHARGING:
            charge += charge_rate;
            if(charge > charge_max)
                charge = charge_max;
            break;
    }

    switch(concentrationState) {
        case CONCENTRATION_STATE_NONE:
            break;
        case CONCENTRATION_STATE_CONCENTRATING:
            concentration += concentration_rate;
            if(concentration > concentration_max)
                concentration = concentration_max;
            break;
    }
}

void Player::frameTell(double* inCharge, double* inConc) {
    *inCharge = charge;
    *inConc = concentration;
}
