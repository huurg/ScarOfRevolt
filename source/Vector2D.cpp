#include "Vector2D.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#include <cmath>

Vector2D::Vector2D() {
    r[0] = 0.0;
    r[1] = 0.0;
}

Vector2D::Vector2D(double inX, double inY) {
    r[0] = inX;
    r[1] = inY;
}

Vector2D::Vector2D(const Vector2D& inV) {
    r[0] = inV.x();
    r[1] = inV.y();
}

double Vector2D::x() const {
    return r[0];
}

double Vector2D::y() const {
    return r[1];
}

void Vector2D::x(double inX) {
    r[0] = inX;
}

void Vector2D::y(double inY) {
    r[1] = inY;
}

double Vector2D::dot(const Vector2D& inV) const {
    double out = x()*inV.x() + y()*inV.y();
    return out;
}

double Vector2D::R2() const {
    return x()*x() + y()*y();
}

double Vector2D::R() const {
    return sqrt(R2());
}

Vector2D Vector2D::operator*(const double inA) const {
    Vector2D out(inA*x(),inA*y());
    return out;
}

double Vector2D::operator*(const Vector2D& inV) const {
    return dot(inV);
}

Vector2D Vector2D::operator+(const Vector2D& inV) const {
    Vector2D out(x()+inV.x(), y()+inV.y());
    return out;
}

Vector2D Vector2D::operator-(const Vector2D& inV) const {
    Vector2D out(x()-inV.x(), y()-inV.y());
    return out;
}

void Vector2D::operator=(const Vector2D& inV) {
    x(inV.x());
    y(inV.y());
}

Vector2D Vector2D::times(const double inA) const {
    Vector2D out(inA*x(),inA*y());
    return out;
}

void Vector2D::render(double inR, double inG, double inB) const {
    glBegin(GL_LINES);
        glColor3d(inR, inG, inB);
        glVertex2d(0,0);
        glVertex2d(x(),y());
    glEnd();
}
