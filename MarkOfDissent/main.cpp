#include <iostream>
using namespace std;

#include "LukeConsoleOut.h"
using namespace LukeConsoleOut;

#include "LCL_RealMatrix.h"

int main(int argc, char** argv) {

    LCL_RealMatrix <10,1> my_mat;
    my_mat(6,0) = 3.7;
    my_mat.print();


    return 0;
}
