#include "Circle.h"

#include "Vector2D.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <cmath>

Circle::Circle() {
    r = new Vector2D();
    v = new Vector2D();
    R = 0.0;
    M = 0.0;
    friction = CIRCLE_DEFAULT_FRICTION;
    elasticity = CIRCLE_DEFAULT_ELASTICITY;
    CCcollision = CIRCLE_DEFAULT_CC_COLLISION;
}

Circle::Circle(double rx, double ry, double vx, double vy, double inR, double inM) {
    r = new Vector2D(rx, ry);
    v = new Vector2D(vx, vy);
    R = inR;
    M = inM;
    friction = CIRCLE_DEFAULT_FRICTION;
    elasticity = CIRCLE_DEFAULT_ELASTICITY;
    CCcollision = CIRCLE_DEFAULT_CC_COLLISION;
}

Circle::~Circle() {
    delete r;
    delete v;
}

void Circle::render() {
    glBegin(GL_LINE_LOOP);
        double dtheta = 2.0*M_PI/(double)CIRCLE_RENDER_VERTICES;
        for(int i = 0; i < CIRCLE_RENDER_VERTICES; i++) {
            double thistheta = i*dtheta;
            double thisX = R*cos(thistheta)+r->x();
            double thisY = R*sin(thistheta)+r->y();
            glVertex2d(thisX,thisY);
        }
    glEnd();
}

void Circle::updateVel(double inVX, double inVY) {
    v->x(v->x()+inVX);
    v->y(v->y()+inVY);
}

void Circle::setVel(double inVX, double inVY) {
    v->x(inVX);
    v->y(inVY);
}

void Circle::updateVel(const Vector2D& inV) {
    v->x(v->x()+inV.x());
    v->y(v->y()+inV.y());
}

void Circle::setVel(const Vector2D& inV) {
    v->x(inV.x());
    v->y(inV.y());
}

void Circle::updatePos(const Vector2D& inR) {
    r->x(r->x()+inR.x());
    r->y(r->y()+inR.y());
}
