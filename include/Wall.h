#ifndef WALL
#define WALL

#include "Vector2D.h"

class Wall {
public:
    Vector2D* a;
    Vector2D* b;
    Wall();
    Wall(double ax, double ay, double bx, double by);
    ~Wall();
    void render();
};
#endif // WALL
