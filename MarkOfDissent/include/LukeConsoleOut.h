#ifndef LUKE_CONSOLE_OUT_HEADER
#define LUKE_CONSOLE_OUT_HEADER

#include <iostream>
using namespace std;

#include <ostream>

#include <ctime>

extern int dout_n;

namespace LukeConsoleOut {
    extern int LOut_Pad;

    ostream& LOut();
    void dout();

    void warning(const char* message, const char* function_name = NULL, const char* class_name = NULL);
    void error(const char* message, const char* function_name = NULL, const char* class_name = NULL); // TODO: add an error array for storing error codes. make error() add to this error code array and output to file log at end of execution.

    double secs(clock_t tic, clock_t toc);
}

#include "LCL_RealMatrix_imp1.h"

#endif
