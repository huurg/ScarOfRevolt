#include "AbilityData.h"

#include "Data.h"
#include <string>
#include <stdio.h>

#include <iostream>
using namespace std;

AbilityData::AbilityData(const AbilityData& inD) {
    index = inD.index;
    name = new string(*(inD.name));
    function_index = inD.function_index;
}

AbilityData::AbilityData(int inIndex, const char* inName, int inFunctionIndex) {
    index = inIndex;
    name = new string(inName);
    function_index = inFunctionIndex;
}

AbilityData::~AbilityData() {
    delete name;
}

void AbilityData::toString(char* inDest) {
    sprintf(inDest, "%20d%20s%20d", index, name->c_str(), function_index);
}

AbilityData& AbilityData::operator=(const AbilityData& inD) {
    index = inD.index;
    name->assign(*(inD.name));
    function_index = inD.function_index;
}
