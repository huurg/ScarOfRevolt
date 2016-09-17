#include "RenderUtils.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <cmath>

void RenderUtils::renderCircle(double r, double x, double y, int v) {
    glBegin(GL_POLYGON);
        //glColor3f(1.0f, 0.0f, 1.0f);
        double dtheta = 2.0*M_PI/(double)v;
        for(int i = 0; i < v; i++) {
            double thistheta = i*dtheta;
            double thisX = r*cos(thistheta)+x;
            double thisY = r*sin(thistheta)+y;
            glVertex2d(thisX,thisY);
        }
    glEnd();
}
