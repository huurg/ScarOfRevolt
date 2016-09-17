#include "WorldScreen.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdlib>
using namespace std;

#include "Vector2D.h"
#include "Environment.h"
#include "Matrix.h"
#include "QuadSolver.h"
#include "AnchorWheel.h"
#include "SingleKeyBinding.h"
#include "ChargeBarUI.h"

WorldScreen::WorldScreen() {
    environment = new Environment(MAX_WALLS, MAX_CIRCLES);
    anchorWheel = new AnchorWheel();
    chargeBinding = new SingleKeyBinding(SDL_SCANCODE_SPACE);
    defendBinding = new SingleKeyBinding(SDL_SCANCODE_LSHIFT);
    chargeBarUI = new ChargeBarUI(100.0,100.0);
}

WorldScreen::~WorldScreen() {
    delete anchorWheel;
    delete environment;
    delete chargeBinding;
    delete defendBinding;
    delete chargeBarUI;
}

void WorldScreen::initListen(int inWidth, int inHeight) {
    screen_width = inWidth;
    screen_height = inHeight;
}

void WorldScreen::frameListen(double inFR) {
    framerate = inFR;
}

void WorldScreen::eventListen(const SDL_Event* inE) {
    if( inE->type == SDL_KEYDOWN) {
        if(inE->key.keysym.sym == SDLK_v) {
            ANGLED_CAMERA = !ANGLED_CAMERA;
        }
    }

    if((inE->type == SDL_MOUSEBUTTONDOWN)&&(inE->button.button == SDL_BUTTON_LEFT)) {
        if(!mouseDown) {
            mouseClick = true;
        }
        mouseDown = true;
    } else if((inE->type == SDL_MOUSEBUTTONUP)&&(inE->button.button == SDL_BUTTON_LEFT)) {
        mouseDown = false;
    }
}

void WorldScreen::init() {
    playerCircle = environment->addCircle(0.0,0.0,0.0,0.0,0.25,2.0);
    int myCircle = environment->addCircle(0.0,5.0,0.0,0.0,0.1,1.0);
    environment->setCircleFriction(myCircle, 0.3);
    environment->loadWalls("walls/boundary.wal");
    environment->loadWalls("walls/house.wal");
}

void WorldScreen::updateSelf() {
    SDL_GetMouseState(&mouseX,&mouseY);
    mouseDirX = 2.0*mouseX/screen_height - (double)screen_width/screen_height;
    mouseDirY = -2.0*mouseY/screen_height + 1.0;
    if(ANGLED_CAMERA) {
        mouseDirY *= (1.0/cos(CAMERA_TILT*M_PI/180.0));
        Matrix mouseDirVec(3,1);
        mouseDirVec.E(0,0,mouseDirX);
        mouseDirVec.E(1,0,mouseDirY);
        Matrix thisRotZ = Matrix::Rotate3DZ(-CAMERA_ROTATE);
        mouseDirVec = thisRotZ*mouseDirVec;
        mouseDirX = mouseDirVec.E(0,0).getReal();
        mouseDirY = mouseDirVec.E(1,0).getReal();
    }
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    for(int i = 0; i < 9; i++) {
        bool thisKeyState = currentKeyStates[anchorWheel->getScanCode(i)];
        anchorWheel->step(i, thisKeyState);

    }
    chargeBinding->step(currentKeyStates[chargeBinding->getBinding()]);
    defendBinding->step(currentKeyStates[defendBinding->getBinding()]);
}

void WorldScreen::broadcast() {
    environment->frameListen(framerate, mouseDown, mouseDirX, mouseDirY, playerCircle, chargeBinding->isUp(), chargeBinding->isDown(), chargeBinding->isHeld());
}

void WorldScreen::updateDown() {
    environment->step();
}

void WorldScreen::updateUp() {
    double thisCharge, thisConc;
    environment->frameTell(&thisCharge, &thisConc);
    chargeBarUI->setCharge(thisCharge);
    chargeBarUI->setConcentration(thisConc);
}

void WorldScreen::render() {
glClear( GL_COLOR_BUFFER_BIT );
    //Set projection matrix
    glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
    glOrtho(-WIDTH_METRES/2.0,WIDTH_METRES/2.0,-screen_height*WIDTH_METRES/screen_width/2.0,screen_height*WIDTH_METRES/screen_width/2.0,NEAR_METRES,FAR_METRES);

    //Set Modelview matrix
    glMatrixMode(GL_MODELVIEW);

    //Render scene
    glPushMatrix();
        if(ANGLED_CAMERA) {
            glRotated(CAMERA_TILT,1,0,0);
            glRotated(CAMERA_ROTATE,0,0,1);
        }

        if(playerCircle>=0) {
            glTranslated(-environment->getCircleX(playerCircle),-environment->getCircleY(playerCircle),0.0);
        }

        renderBackground();

        environment->render();
    glPopMatrix();

    //Render overlay
    anchorWheel->render();
    chargeBarUI->render();

    glMatrixMode(GL_MODELVIEW);
}

void WorldScreen::step() {
    updateSelf();
    broadcast();
    updateDown();
    updateUp();
    render();
}

void WorldScreen::renderBackground() {
    glBegin(GL_QUADS);
        for(int i = 0; i < 26; i++) {
            for(int j = 0; j < 18; j++) {
                glColor3d(0.2*cos(i*j)*cos(i*j),0.2,0.2*sin(i*j)*sin(i*j));
                glVertex2d(-13+i,-9+j);
                glColor3d(0.2*cos(i*j+1)*cos(i*j+1),0.2,0.2*sin(i*j+1)*sin(i*j+1));
                glVertex2d(-13+i+1,-9+j);
                glColor3d(0.2*cos(i*j+2)*cos(i*j+2),0.2,0.2*sin(i*j+2)*sin(i*j+2));
                glVertex2d(-13+i+1,-9+j+1);
                glColor3d(0.2*cos(i*j+3)*cos(i*j+3),0.2,0.2*sin(i*j+3)*sin(i*j+3));
                glVertex2d(-13+i,-9+j+1);
            }
        }
    glEnd();
}
