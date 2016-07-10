#ifndef QUAD_SOLVER
#define QUAD_SOLVER

class QuadSolver {
private:
    double x1;
    double x2;
    double A;
    double B;
    double C;
    int outcome; //0 for no real solution; 1 for one real solution (placed in x1); 2 for two real solutions (x1 < x2); -1 for out of date (not yet solved for given params);
public:
    QuadSolver(double inA, double inB, double inC);
    int solve(); //returns outcome
    double getX1() const;
    double getX2() const;
    int getOutcome() const;
    void setParams(double inA, double inB, double inC);
};

#endif // QUAD_SOLVER
