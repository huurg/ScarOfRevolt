/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL OpenGL, standard IO, and, strings
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

//Screen dimension + other constants
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
const double WIDTH_METRES = 10.0;
const int MAX_WALLS = 50;
const int MAX_CIRCLES = 100;
bool ANGLED_CAMERA = true;
double CAMERA_TILT = 50.0;
double CAMERA_ROTATE = 45.0;

//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes matrices and clear color
bool initGL();

//Input handler
void handleKeys( unsigned char key, int x, int y );

//Per frame update
void update();

//Renders quad to the screen
void render();

void renderBackground();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Render flag
bool gRenderQuad = false;

//Environment object
Environment* gEnvironment = NULL;

//Framerate analysis
int gFrames = 0;
int gRuntime = 0;
double gFramerate = 60.0;
const int FRAMES_POLLED = 60;

//Game global variables
int gPlayerCircle = -1;

//Mouse state globals
bool gMouseDown = false;
bool gMouseClick = false;
int gMouseX = 0.0;
int gMouseY = 0.0;
double gMouseDirX = 0.0;
double gMouseDirY = 0.0;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext( gWindow );
			if( gContext == NULL )
			{
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 )
				{
					printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				}

				//Initialize OpenGL
				if( !initGL() )
				{
					printf( "Unable to initialize OpenGL!\n" );
					success = false;
				}
			}
		}
	}
    gEnvironment = new Environment(MAX_WALLS, MAX_CIRCLES);
    /*
    for(int i = 0; i < 99; i++) {
        gEnvironment->addCircle(5.0*(double)rand()/RAND_MAX-2.5,2.0*(double)rand()/RAND_MAX+4,5.0*(double)rand()/RAND_MAX-2.5,5.0*(double)rand()/RAND_MAX-2.5,0.1,1.0);
    }
    */
    gPlayerCircle = gEnvironment->addCircle(0.0,0.0,0.0,0.0,0.25,2.0);
    int myCircle = gEnvironment->addCircle(0.0,5.0,0.0,0.0,0.1,1.0);
    gEnvironment->setCircleFriction(myCircle, 0.3);
    /*
    double boundary[16] = {-9.0, -1.0, -9.0, 5.0, -1.0, 6.0, 8.0, 4.0, 4.0, 0.0, 9.0, -6.0, 2.0, -6.0, -7.0, -4.0};
    gEnvironment->loadWalls(boundary, 8);

    double box[8] = {-2.0,2.0,-2.0,-2.0,2.0,-2.0,2.0,2.0};
    gEnvironment->loadWalls(box, 4);
    */
    gEnvironment->loadWalls("walls/boundary.wal");
    gEnvironment->loadWalls("walls/house.wal");
	return success;
}

bool initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}

	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}

	//Initialize clear color
	glClearColor( 0.f, 0.f, 0.f, 1.f );

	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}
    glViewport(-SCREEN_WIDTH/2, -SCREEN_HEIGHT/2, SCREEN_WIDTH*2, SCREEN_HEIGHT*2);
	return success;
}

void handleKeys( unsigned char key, int x, int y )
{
	//Toggle quad
	if( key == 'q' )
	{
		gRenderQuad = !gRenderQuad;
	}
}

void update()
{
	//No per frame update needed
	gEnvironment->step(gFramerate, gMouseDown, gMouseDirX, gMouseDirY, gPlayerCircle);
	if(gMouseClick) {
        cout << "MouseX = " << gMouseX << "\tMouseY = " << gMouseY << endl;
        cout << "DirX = " << gMouseDirX << "\tDirY = " << gMouseDirY << endl;
	}
}

void render()
{
    glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

    glScaled(1.0/WIDTH_METRES,1.0/WIDTH_METRES,1.0/WIDTH_METRES);
    glScaled(1,(double)SCREEN_WIDTH/(double)SCREEN_HEIGHT,1);

    if(ANGLED_CAMERA) {
        glRotated(CAMERA_TILT,1,0,0);
        glRotated(CAMERA_ROTATE,0,0,1);
    }

    if(gPlayerCircle>=0) {
        glTranslated(-gEnvironment->getCircleX(gPlayerCircle),-gEnvironment->getCircleY(gPlayerCircle),0.0);
    }
    glMatrixMode(GL_MODELVIEW);

	//Clear color buffer
	glClear( GL_COLOR_BUFFER_BIT );

    renderBackground();

    gEnvironment->render();

    glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

    glScaled((double)SCREEN_HEIGHT/(double)SCREEN_WIDTH/2,1.0/2,1.0/2);
    //Overlay stuff here
    /*
    glPushMatrix();
        if(ANGLED_CAMERA) {
            glRotated(CAMERA_TILT,1,0,0);
            glRotated(CAMERA_ROTATE,0,0,1);
        }
        Vector2D playerV(gEnvironment->getCircleVX(gPlayerCircle)/7.0,gEnvironment->getCircleVY(gPlayerCircle)/7.0);
        playerV.render(0.0,0.0,1.0);
    glPopMatrix();
    */
    /*
    Vector2D mouseDir(gMouseDirX,gMouseDirY);
    mouseDir.render(1.0,0.0,0.0);
    */
    glMatrixMode(GL_MODELVIEW);
}

void close()
{
    delete gEnvironment;
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		//Enable text input
		SDL_StartTextInput();

        gRuntime = clock();

		//While application is running
		while( !quit )
		{
		    gMouseClick = false;
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}
				if( e.type == SDL_KEYDOWN) {
                    if(e.key.keysym.sym == SDLK_v) {
                        ANGLED_CAMERA = !ANGLED_CAMERA;
                    }
				}

				if((e.type == SDL_MOUSEBUTTONDOWN)&&(e.button.button == SDL_BUTTON_LEFT)) {
                    if(!gMouseDown) {
                        gMouseClick = true;
                    }
                    gMouseDown = true;
				} else if((e.type == SDL_MOUSEBUTTONUP)&&(e.button.button == SDL_BUTTON_LEFT)) {
                    gMouseDown = false;
				}
				SDL_GetMouseState(&gMouseX,&gMouseY);
				gMouseDirX = 2.0*gMouseX/SCREEN_HEIGHT - (double)SCREEN_WIDTH/SCREEN_HEIGHT;
                gMouseDirY = -2.0*gMouseY/SCREEN_HEIGHT + 1.0;
                if(ANGLED_CAMERA) {
                    gMouseDirY *= (1.0/cos(CAMERA_TILT*M_PI/180.0));
                    Matrix mouseDirVec(3,1);
                    mouseDirVec.E(0,0,gMouseDirX);
                    mouseDirVec.E(1,0,gMouseDirY);
                    Matrix thisRotZ = Matrix::Rotate3DZ(-CAMERA_ROTATE);
                    mouseDirVec = thisRotZ*mouseDirVec;
                    gMouseDirX = mouseDirVec.E(0,0).getReal();
                    gMouseDirY = mouseDirVec.E(1,0).getReal();
                }
			}

			//Render quad
			render();
            update();
			//Update screen
			SDL_GL_SwapWindow( gWindow );

			gFrames++;
			if(!(gFrames%FRAMES_POLLED)){
                gFramerate = (double)gFrames/((clock() - gRuntime)/(double)CLOCKS_PER_SEC);
                gFrames = 0;
                gRuntime = clock();
                cout << "Framerate: " << gFramerate << " fps" << endl;
			}
		}

		//Disable text input
		SDL_StopTextInput();
	}

	//Free resources and close SDL
	close();

	return 0;
}

void renderBackground() {
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
