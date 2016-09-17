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
#include "AnchorWheel.h"
#include "SingleKeyBinding.h"
#include "ChargeBarUI.h"
#include "WorldScreen.h"
#include "Data.h"
#include "AnchorDatabase.h"
#include "AbilityDatabase.h"
#include "SpellingDatabase.h"
#include "CastingDatacentre.h"
#include "PE_PhysicsEnvironment.h"
#include "CL_AnchorList.h"

//Screen dimension + other constants
const int SCREEN_WIDTH = 1900;
const int SCREEN_HEIGHT = 1000;

//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes matrices and clear color
bool initGL();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Framerate analysis
int gFrames = 0;
int gRuntime = 0;
double gFramerate = 60.0;
const int FRAMES_POLLED = 60;

//Keep this
WorldScreen* gWorldScreen = NULL;

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

    gWorldScreen = new WorldScreen();
    gWorldScreen->initListen(SCREEN_WIDTH, SCREEN_HEIGHT);
    gWorldScreen->init();

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

	return success;
}

void close()
{
    delete gWorldScreen;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] )
{

    CastingDatacentre myCD;
    myCD.parseCommandString("(-4-1-4-1-)-(-3-12-12-)-2-1-2-(-)-1-4-8-", NULL);

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
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}
				gWorldScreen->eventListen(&e);
			}

            gWorldScreen->frameListen(gFramerate);
            gWorldScreen->step();

			//Update screen
			SDL_GL_SwapWindow( gWindow );

            //Frame rate monitoring
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
