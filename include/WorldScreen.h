#ifndef WORLD_SCREEN_HEADER
#define WORLD_SCREEN_HEADER

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

#include "Environment.h"
#include "AnchorWheel.h"
#include "SingleKeyBinding.h"
#include "ChargeBarUI.h"

class WorldScreen {
    private:
        //Game constants
        const int MAX_WALLS = 50;
        const int MAX_CIRCLES = 100;
        bool ANGLED_CAMERA = true;
        double CAMERA_TILT = 60.0;
        double CAMERA_ROTATE = 45.0;
        const double WIDTH_METRES = 20.0;
        const double NEAR_METRES = -100.0;
        const double FAR_METRES = 70.0;

        int screen_height;
        int screen_width;
        int playerCircle = -1;
        AnchorWheel* anchorWheel = NULL;
        Environment* environment = NULL;
        SingleKeyBinding* chargeBinding = NULL;
        SingleKeyBinding* defendBinding = NULL;
        ChargeBarUI* chargeBarUI = NULL;

        //State variables
        bool mouseDown = false;
        bool mouseClick = false;
        int mouseX = 0.0;
        int mouseY = 0.0;
        double mouseDirX = 0.0;
        double mouseDirY = 0.0;
        double framerate;


        //The "Step-Stage" methods.
        void updateSelf();  //1. Updates instance of WorldScreen's own state variables.
        void broadcast();   //2. Cascade updated state variables down to child objects.
        void updateDown();  //3. Call all relevant update methods of child objects. These will change their respective state variables according to game rules.
        void updateUp();    //4. Retrieves updates that cascade back up from lowest child level to WorldScreen.
        void render();      //5. Call all relevant render methods of child objects. These will render these objects to screen.

        //Temporary methods
        void renderBackground();
    public:
        WorldScreen();
        ~WorldScreen();

        void initListen(int inWidth, int inHeight); //Must be set before main loop begins.
        void init(); //Initial set-up of state variables

        void frameListen(double inFR);
        void eventListen(const SDL_Event* inE);
        void step(); //Consists of calling the 5 "step-stage" methods in order.

};

#endif // WORLD_SCREEN_HEADER
