#include "QuadSolver.h"

#include <cmath>

QuadSolver::QuadSolver(double inA, double inB, double inC) {
    A = inA;
    B = inB;
    C = inC;
    x1 = x2 = 0.0;
    outcome = -1;
}

int QuadSolver::solve() {
    double xp,xm,r;
    r = B*B - 4.0*A*C;
    if(r>0.0) {
        xm = (-B-sqrt(r))/(2*A);
        xp = (-B+sqrt(r))/(2*A);
        if(xm<=xp) {
            x1 = xm;
            x2 = xp;
        } else {
            x1 = xp;
            x2 = xm;
        }
        outcome = 2;
    } else {
        outcome = 0;
    }
    return outcome;
}

double QuadSolver::getX1() const {
    return x1;
}

double QuadSolver::getX2() const {
    return x2;
}

int QuadSolver::getOutcome() const {
    return outcome;
}

void QuadSolver::setParams(double inA, double inB, double inC) {
    A = inA;
    B = inB;
    C = inC;
    outcome = -1;
}
