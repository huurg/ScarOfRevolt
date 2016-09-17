#ifndef ANCHOR_DATA_HEADER
#define ANCHOR_DATA_HEADER

#include "Data.h"
#include <string>

#include <iostream>
using namespace std;

class AnchorData: public Data {
private:
    int index;
    char type; //c|f|u = control|form|unity
    string* name;
    string* shortname;
    bool assignable;
public:
    AnchorData(const AnchorData& inD);
    AnchorData(int inIndex, char inType, const char* inName, const char* inShortname, bool inAss);
    ~AnchorData();

    void toString(char* inDest);
    AnchorData& operator=(const AnchorData& inD);

    int getIndex() const;
    char getType() const;
};

#endif
