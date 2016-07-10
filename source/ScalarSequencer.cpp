#include "ScalarSequencer.h"

ScalarSequencer::ScalarSequencer(int inN) {
    sequence = new double[inN];
    durations_time = new double[inN];
    durations_frames = new int[inN];
}

ScalarSequencer::~ScalarSequencer() {
    delete [] sequence;
    delete [] durations_time;
    delete [] durations_frames;
}

int ScalarSequencer::addValue(double inV, double inD) {
    ;
}

int ScalarSequencer::addValue(double inV, int inD) {
    ;
}
