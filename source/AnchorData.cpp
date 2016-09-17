#include "AnchorData.h"

#include "Data.h"
#include <string>
#include <stdio.h>

#include <iostream>
using namespace std;

AnchorData::AnchorData(const AnchorData& inD) {
    index = inD.index;
    type = inD.type;
    name = new string(*(inD.name));
    shortname = new string(*(inD.shortname));
    assignable = inD.assignable;
}

AnchorData::AnchorData(int inIndex, char inType, const char* inName, const char* inShortname, bool inAss) {
    index = inIndex;
    type = inType;
    name = new string(inName);
    shortname = new string(inShortname);
    assignable = inAss;
}

AnchorData::~AnchorData() {
    delete name;
    delete shortname;
}

void AnchorData::toString(char* inDest) {
    sprintf(inDest, "%20d%20c%20s%20s%20d", index, type, name->c_str(), shortname->c_str(), assignable);
}

AnchorData& AnchorData::operator=(const AnchorData& inD) {
    index = inD.index;
    type = inD.type;
    name->assign(*(inD.name));
    shortname->assign(*(inD.shortname));
    assignable = inD.assignable;
}

int AnchorData::getIndex() const {
    return index;
}

char AnchorData::getType() const {
    return type;
}
