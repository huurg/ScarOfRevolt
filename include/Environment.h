#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "Wall.h"
#include "Circle.h"
#include "Vector2D.h"

const double COLLISION_MARGIN = 0.01;

class Environment {
private:
    int W, Nw;
    Wall** walls;
    int C, Nc;
    Circle** circles;
    bool visible;
    bool paused;

    //Interaction methods
    bool collisionCW(int indexC, int indexW, double inFR);
    bool collisionCA(int indexC, int indexW, double inFR);
    bool collisionCB(int indexC, int indexW, double inFR);
    bool collisionCC(int indexC1, int indexC2, double inFR);
    //CW collision vectors
    Vector2D* t_(int indexC, int indexW) const;
    Vector2D* u_(int indexC, int indexW) const;
    //CC collision methods
    void updateCCvels(int indexC1, int indexC2, double inFR);
public:
    Environment(int inNw, int inNc);
    ~Environment();
    int addWall(double ax, double ay, double bx, double by);
    int addCircle(double rx, double ry, double vx, double vy, double inR, double inM);
    void removeWall(int index);
    void removeCircle(int index);
    void step(double inFR, bool inMD, double inMX, double inMY, int inPC); //args: framerate, mousedown, mouseX, mouseY, playercircle
    void render();
    void pause();
    void unpause();
    double getCircleX(int index) const;
    double getCircleY(int index) const;
    double getCircleVX(int index) const;
    double getCircleVY(int index) const;
    bool setCircleMass(int index, double inM);
    bool setCircleFriction(int index, double inF);
    bool setCircleElasticity(int index, double inE);
    bool setCircleCCC(int index, bool inCCC);
    void loadWalls(double* inWs,int inMax);
    void loadWalls(const char* inFilename);
};

#endif // ENVIRONMENT
