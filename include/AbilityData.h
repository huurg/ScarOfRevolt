#ifndef ABILITY_DATA_HEADER
#define ABILITY_DATA_HEADER

#include "Data.h"
#include <string>

#include <iostream>
using namespace std;

class AbilityData: public Data {
    private:
        int index;
        string* name;
        int function_index;
    public:
        AbilityData(const AbilityData& inD);
        AbilityData(int inIndex, const char* inName, int inFunctionIndex);
        ~AbilityData();

        void toString(char* inDest);
        AbilityData& operator=(const AbilityData& inD);
};

#endif // ABILITY_DATA_HEADER
