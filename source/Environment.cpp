#include "Environment.h"

#include "Wall.h"
#include "Circle.h"
#include "QuadSolver.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <iostream>
using namespace std;

#include <fstream>

#include <cmath>

#include "Vector2D.h"
#include "Player.h"

Environment::Environment(int inNw, int inNc) {
    Nw = inNw;
    Nc = inNc;
    walls = new Wall*[Nw];
    circles = new Circle*[Nc];
    for(int i = 0; i < Nw; i++) walls[i] = NULL;
    for(int i = 0; i < Nc; i++) circles[i] = NULL;
    W = 0;
    C = 0;
    visible = true;
    player = new Player();
}

Environment::~Environment() {
    for(int i = 0; i < Nw; i++) {
        if(walls[i]!=NULL) {
            delete walls[i];
        }
    }
    delete[] walls;
    for(int i = 0; i < Nc; i++) {
        if(circles[i]!=NULL) {
            delete circles[i];
        }
    }
    delete[] circles;
    delete player;
}

int Environment::addWall(double ax, double ay, double bx, double by) {
    int i = 0;
    while((walls[i]!=NULL)&&(i<Nw)) {
        i++;
    }
    if(i!=Nw) {
        walls[i] = new Wall(ax, ay, bx, by);
        W++;
        return i;
    } else {
        cout << "Can't add Wall. Wall list full" << endl;
        return -1;
    }
}

int Environment::addCircle(double rx, double ry, double vx, double vy, double inR, double inM) {
    int i = 0;
    while((circles[i]!=NULL)&&(i<Nc)) {
        i++;
    }
    if(i!=Nc) {
        circles[i] = new Circle(rx, ry, vx, vy, inR, inM);
        C++;
        return i;
    } else {
        cout << "Can't add Circle. Circle list full" << endl;
        return -1;
    }
}

void Environment::removeWall(int index) {
    if(walls[index]!=NULL) {
        delete walls[index];
        walls[index] = NULL;
        W--;
    }
}

void Environment::removeCircle(int index) {
    if(circles[index]!=NULL) {
        delete circles[index];
        circles[index] = NULL;
        C--;
    }
}

void Environment::render() {
    if(visible) {
        glLineWidth(3.0f);
        glColor3f(1.0,1.0,1.0);

        for(int i = 0; i < Nc; i++) {
            if(circles[i]!=NULL) {
                circles[i]->render();
            }
        }

        glColor3f(0.f,1.0f,0.f);

        for(int i = 0; i < Nw; i++) {
            if(walls[i]!=NULL){
                walls[i]->render();
            }
        }
    }
}

void Environment::pause() {
    paused = true;
}

void Environment::unpause() {
    paused = false;
}

void Environment::frameListen(double inFR, bool inMD, double inMX, double inMY, int inPC, bool inUp, bool inDown, bool inHeld) {
    framerate = inFR;
    mousedown = inMD;
    mouseX = inMX;
    mouseY = inMY;
    playercircle = inPC;
    player->frameListen(inUp, inDown, inHeld);
}

void Environment::step() {
    if(!paused) {
        for(int i = 0; i < Nc; i++) {
            Circle* thisC = circles[i];
            if(thisC!=NULL) {
                for(int j = i+1; j < Nc; j++) {
                    if(i!=j) {
                        updateCCvels(i,j,framerate);
                    }
                }
                for(int j = 0; j < Nw; j++) {
                    if(walls[j]!=NULL) {
                        //Make coefficient of restitution property of each wall and each circle
                        double thisCoeffRest = 0.5;
                        if(collisionCW(i,j,framerate)) {
                            Vector2D* thisU = u_(i,j);
                            Vector2D* thisT = t_(i,j);
                            Vector2D* thisR = thisC->r;
                            Vector2D temp0 = (*thisT) - (*thisR);
                            if(temp0.dot(*thisU)>=0.0) {
                                Vector2D temp = (*thisU)*(-(1.0+thisCoeffRest)*thisC->v->dot(*thisU));
                                thisC->updateVel(temp);
                                thisC->updatePos(*thisT-*thisU*(thisC->R+COLLISION_MARGIN)-*(thisC->r));
                                delete thisU;
                                delete thisT;
                            }
                        } else if(collisionCA(i,j,framerate)) {
                            Vector2D* thisA = walls[j]->a;
                            Vector2D* thisCv = circles[i]->r;
                            Vector2D thisU = (*thisA-*thisCv)*(1.0/(*thisA-*thisCv).R());
                            circles[i]->updateVel(thisU*(-(1.0+thisCoeffRest)*circles[i]->v->dot(thisU)));
                            circles[i]->updatePos(*thisA-thisU*(thisC->R+COLLISION_MARGIN)-*thisCv);
                        } else if(collisionCB(i,j,framerate)) {
                            Vector2D* thisB = walls[j]->b;
                            Vector2D* thisCv = circles[i]->r;
                            Vector2D thisU = (*thisB-*thisCv)*(1.0/(*thisB-*thisCv).R());
                            circles[i]->updateVel(thisU*(-(1.0+thisCoeffRest)*circles[i]->v->dot(thisU)));
                            circles[i]->updatePos(*thisB-thisU*(thisC->R+COLLISION_MARGIN)-*thisCv);
                        }
                    }
                }
                //Friction decay

                if((i==playercircle)&&(mousedown)&&(sqrt(mouseX*mouseX+mouseY*mouseY)>0.05)) {
                    double xd = mouseX;
                    double yd = mouseY;
                    Vector2D rd(xd*1.0, yd*1.0);
                    if(rd.R()>1.0) {
                        rd = rd*(1.0/rd.R());
                    }
                    //Make the following 3 constants properties of the player
                    double thisAccel = 10.0;
                    double thisDecel = 10.0;
                    double thisMaxSpeed = 7.0;

                    Vector2D ud(-yd/rd.R(),xd/rd.R());
                    double temp0;
                    if(thisC->v->R()>0.01) {
                        temp0 = thisDecel*(1-(thisC->v->dot(rd))/thisC->v->R());
                    } else {
                        temp0 = 0.0;
                    }
                    double temp = (thisAccel+temp0)*(1.0-(thisC->v->dot(rd))/(thisMaxSpeed*rd.R2()))/(rd.R()*framerate);
                    //cout << "xd = " << xd << "\tyd = " << yd << "\ttemp = " << temp << endl;
                    thisC->updateVel(rd*temp/*-ud*(thisC->v->dot(ud))*(100.0/inFR)*/);
                    thisC->setVel(rd*(thisC->v->dot(rd))*(1.0/rd.R2()));
                } else {
                    //Make friction a property of both the environment and each circle. Best value for player is 5.0
                    double thisFriction = sqrt(5.0);
                    thisC->updateVel(*(thisC->v)*(-thisFriction*thisC->friction/framerate));
                }

                //Update position
                Vector2D* thisV = thisC->v;
                Vector2D temp2 = thisV->times(1.0/framerate);
                thisC->updatePos(temp2);
            }
        }
        player->step();
    }
}

void Environment::frameTell(double* inCharge, double* inConc) {
    player->frameTell(inCharge, inConc);
}

bool Environment::collisionCW(int indexC, int indexW, double inFR) {
    Vector2D* myT = t_(indexC, indexW);
    Vector2D* thisC = circles[indexC]->r;
    Vector2D* thisA = walls[indexW]->a;
    Vector2D* thisB = walls[indexW]->b;
    Vector2D thisC2 = *(circles[indexC]->r)+*(circles[indexC]->v)*(2.0/inFR);
    double thisR = circles[indexC]->R;
    double bma = (*thisB - *thisA).R();
    double tma = (*myT - *thisA).R();
    double tmb = (*myT - *thisB).R();
    bool out = false;
    if((((*myT-*thisC).R()<thisR)||((*myT-thisC2).R()<thisR))&&(tma<bma)&&(tmb<bma)) {
        out = true;
    } else {
        double thisM = (thisB->x()-thisA->x())/(thisB->y()-thisA->y());
        double lambda1 = (thisA->x()-thisC->x()+thisM*(thisC->y()-thisA->y()))/(thisC2.x()-thisC->x()-thisM*(thisC2.y()-thisC->y()));
        double lambda2 = (thisC->y()-thisA->y()+lambda1*(thisC2.y()-thisC->y()))/(thisB->y()-thisA->y());
        if((fabs(lambda1-0.5)<0.5)&&(fabs(lambda2-0.5)<0.5)) {
            out = true;
        }
    }
    delete myT;
    return out;
}

bool Environment::collisionCA(int indexC, int indexW, double inFR) {
    Vector2D* thisC = circles[indexC]->r;
    Vector2D thisC2 = *(thisC)+*(circles[indexC]->v)*(1.0/inFR);
    Vector2D* thisA = walls[indexW]->a;
    bool out = false;
    if(((*thisA-*thisC).R()<circles[indexC]->R)||((*thisA-thisC2).R()<circles[indexC]->R)) {
        out = true;
    }
    return out;
}

bool Environment::collisionCB(int indexC, int indexW, double inFR) {
    Vector2D* thisC = circles[indexC]->r;
    Vector2D thisC2 = *(thisC)+*(circles[indexC]->v)*(1.0/inFR);
    Vector2D* thisB = walls[indexW]->b;
    bool out = false;
    if(((*thisB-*thisC).R()<circles[indexC]->R)||((*thisB-thisC2).R()<circles[indexC]->R)) {
        out = true;
    }
    return out;
}

bool Environment::collisionCC(int indexC1, int indexC2, double inFR) {
    Vector2D* thisC1 = circles[indexC1]->r;
    Vector2D* thisC2 = circles[indexC2]->r;
    Vector2D thisC1p = *thisC1 + *(circles[indexC1]->v)*(1.0/inFR);
    Vector2D thisC2p = *thisC2 + *(circles[indexC2]->v)*(1.0/inFR);
    double dist = circles[indexC1]->R+circles[indexC2]->R;
    bool out = ((*thisC2-*thisC1).R()<dist)||((thisC2p-thisC1p).R()<dist);
    return out;
}

Vector2D* Environment::t_(int indexC, int indexW) const {
    Wall* thisW = walls[indexW];
    Circle* thisCi = circles[indexC];
    if((thisW==NULL)||(thisCi==NULL)||(indexC>=Nc)||(indexW>=Nw)) return NULL;
    Vector2D* thisA = thisW->a;
    Vector2D* thisB = thisW->b;
    Vector2D* thisC = thisCi->r;
    Vector2D temp1 = (*thisC-*thisA);
    Vector2D u_ = (*thisB-*thisA)*(1.0/(*thisB-*thisA).R());
    Vector2D temp2 = u_*(temp1*u_) + *thisA;
    return new Vector2D(temp2);
}

Vector2D* Environment::u_(int indexC, int indexW) const {
    Vector2D* thisA = walls[indexW]->a;
    Vector2D* thisB = walls[indexW]->b;
    Vector2D temp(thisA->y()-thisB->y(),thisB->x()-thisA->x());
    double thisR = temp.R();
    return new Vector2D(temp.x()/thisR,temp.y()/thisR);
}

double Environment::getCircleX(int index) const {
    if(circles[index]!=NULL) {
        return circles[index]->r->x();
    } else {
        return 0.0;
    }
}

double Environment::getCircleY(int index) const {
    if(circles[index]!=NULL) {
        return circles[index]->r->y();
    } else {
        return 0.0;
    }
}

double Environment::getCircleVX(int index) const {
    if(circles[index]!=NULL) {
        return circles[index]->v->x();
    } else {
        return 0.0;
    }
}

double Environment::getCircleVY(int index) const {
    if(circles[index]!=NULL) {
        return circles[index]->v->y();
    } else {
        return 0.0;
    }
}

void Environment::loadWalls(double* inWs, int inMax) {
    int i = 0;
    while((i<inMax)&&(W<Nw)){
        addWall(inWs[2*i],inWs[2*i+1],inWs[(2*i+2)%(2*inMax)],inWs[(2*i+3)%(2*inMax)]);
        i++;
    }
}

void Environment::loadWalls(const char* inFilename) {
    ifstream myFile(inFilename);
    int MaxWalls;
    myFile >> MaxWalls;
    int i = 0;
    double thisX, thisY, oldX, oldY, firstX, firstY;
    myFile >> thisX;
    myFile >> thisY;
    firstX = thisX;
    firstY = thisY;
    while((i<(MaxWalls-1))&&(W<Nw)){
        oldX = thisX;
        oldY = thisY;
        myFile >> thisX;
        myFile >> thisY;
        addWall(oldX,oldY,thisX,thisY);
        i++;
    }
    addWall(thisX,thisY,firstX,firstY);
    myFile.close();
}

void Environment::updateCCvels(int indexC1, int indexC2, double inFR) {
    Circle* thisC1 = circles[indexC1];
    Circle* thisC2 = circles[indexC2];
    if((thisC1!=NULL)&&(thisC2!=NULL)) {
        if(thisC1->CCcollision&&thisC2->CCcollision&&collisionCC(indexC1, indexC2, inFR)) {
            Vector2D thisU = *(thisC2->r)-*(thisC1->r);
            double thisUR = thisU.R();
            thisU.x(thisU.x()/thisUR);  thisU.y(thisU.y()/thisUR);
            Vector2D thisW(-thisU.y(),thisU.x());
            double thisA = thisC1->M+thisC2->M;
            double thisB = -2.0*thisC2->M*(thisC1->v->dot(thisU)+thisC2->v->dot(thisU));
            double thisC = thisC1->M*pow(thisC1->v->dot(thisW),2)+thisC2->M*(pow(thisC1->v->dot(thisU)+thisC2->v->dot(thisU),2)+pow(thisC2->v->dot(thisW),2))-thisC1->M*thisC1->v->R2()-thisC2->M*thisC2->v->R2();
            QuadSolver mySolver(thisA,thisB,thisC);
            mySolver.solve();
            double thisV1p = mySolver.getX1();
            double thisV2p = thisC1->v->dot(thisU)+thisC2->v->dot(thisU)-thisV1p;
            thisC1->updateVel(thisU*(thisV1p-thisC1->v->dot(thisU)));
            thisC2->updateVel(thisU*(thisV2p-thisC2->v->dot(thisU)));
        }
    }
}
bool Environment::setCircleMass(int index, double inM) {
    if(circles[index]!=NULL) {
        circles[index]->M = inM;
        return true;
    } else {
        return false;
    }
}
bool Environment::setCircleFriction(int index, double inF) {
    if(circles[index]!=NULL) {
        circles[index]->friction = inF;
        return true;
    } else {
        return false;
    }
}

bool Environment::setCircleElasticity(int index, double inE) {
    if(circles[index]!=NULL) {
        circles[index]->elasticity = inE;
        return true;
    } else {
        return false;
    }
}
bool Environment::setCircleCCC(int index, bool inCCC) {
    if(circles[index]!=NULL) {
        circles[index]->CCcollision = inCCC;
        return true;
    } else {
        return false;
    }
}
