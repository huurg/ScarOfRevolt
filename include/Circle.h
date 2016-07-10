/*
Circle class represents any physical object that is subject to time evolution based on classical physics.
This should only contain properties and methods that are relevant to the abstract physics of the entity
(i.e. nothing related to what the object represents in the game like player stats or damage potential etc.)
The class should however contain parameters that determine how the physics should behave (elasticity, friction etc.)
*/

#ifndef CIRCLE
#define CIRCLE

#include "Vector2D.h"
#include <cmath>

const int CIRCLE_RENDER_VERTICES = 20;
const double CIRCLE_DEFAULT_FRICTION = sqrt(5.0);
const double CIRCLE_DEFAULT_ELASTICITY = sqrt(0.5);
const bool CIRCLE_DEFAULT_CC_COLLISION = true;

class Circle {
public:
    //Properties
    Vector2D* r;
    double R;
    double M;
    Vector2D* v;
    double friction;
    double elasticity;
    bool CCcollision;

    //Constructor/destructor
    Circle();
    Circle(double rx, double ry, double vx, double vy, double inR, double inM);
    ~Circle();

    //Member functions
    void render();

    void updateVel(double inVX, double inVY);
    void updateVel(const Vector2D& inV);
    void updatePos(const Vector2D& inR);
    void setVel(double inVX, double inVY);
    void setVel(const Vector2D& inV);
};

#endif // CIRCLE
