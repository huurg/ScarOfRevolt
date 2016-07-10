#ifndef SCALAR_SEQUENCER_INCLUDE
#define SCALAR_SEQUENCER_INCLUDE

class ScalarSequencer {
private:
    double value;
    double* sequence;
    double* durations_time;
    int* durations_frames;

    double global_frequency;
    double global_amplitude;
public:
    ScalarSequencer(int inN);
    ~ScalarSequencer();
    int addValue(double inV, double inD);
    int addValue(double inV, int inD);
    loadFromFile(const char* inFilename, const char inDType);

};

#endif // SCALAR_SEQUENCER_INCLUDE
