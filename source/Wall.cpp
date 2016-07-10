#include "Wall.h"

#include "Vector2D.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <iostream>
using namespace std;

Wall::Wall() {
    a = new Vector2D();
    b = new Vector2D();
}

Wall::Wall(double ax, double ay, double bx, double by) {
    a = new Vector2D(ax,ay);
    b = new Vector2D(bx,by);
}

Wall::~Wall() {
    delete a;
    delete b;
}

void Wall::render() {
    glBegin(GL_LINES);
        glVertex2d(a->x(),a->y());
        glVertex2d(b->x(),b->y());
    glEnd();
}
