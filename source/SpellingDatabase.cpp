#include "SpellingDatabase.h"

#include <cstring>
#include <cstdlib>
#include <stdio.h>

#include <iostream>
using namespace std;

#include "SpellingData.h"

SpellingDatabase::SpellingDatabase(int inMax): Database(inMax) {
    ;
}

void SpellingDatabase::addData(const char* inLine) {
    char thisLine[MAX_LINE_LENGTH];
    strcpy(thisLine, inLine);
    char* thisToken = NULL;

    int thisIndex = -1;
    int thisAbilityIndex = -1;
    int thisAnchorIndex = -1;
    char thisSpelling[32];

    int nargs = 0;

    thisToken = strtok(thisLine,Database::TOKEN_DELIMITERS);
    if(thisToken) {
        thisIndex = atoi(thisToken);
        nargs++;
    }
    thisToken = strtok(NULL,Database::TOKEN_DELIMITERS);
    if(thisToken) {
        thisAbilityIndex = atoi(thisToken);
        nargs++;
    }
    thisToken = strtok(NULL,Database::TOKEN_DELIMITERS);
    if(thisToken) {
        thisAnchorIndex = atoi(thisToken);
        nargs++;
    }
    thisToken = strtok(NULL,Database::TOKEN_DELIMITERS);
    if(thisToken) {
        strcpy(thisSpelling, thisToken);
        nargs++;
    }

    if((nargs==4)&&(n_data<max_data)) {
        data[n_data] = new SpellingData(thisIndex,thisAbilityIndex,thisAnchorIndex,thisSpelling);
        n_data++;
    }
}

void SpellingDatabase::printTitle() {
    printf("%20s%20s%20s%20s%20s", "Index", "Ability_Index", "Anchor_Index", "Spelling", "No_Anchors");
}

const SpellingData* SpellingDatabase::searchSpelling(const int* inSAL, int inNAnchors, int& outPos) {
    SpellingData* out = NULL;
    bool found = false;
    for(int i = 0; (!found)&&(i < n_data); i++) {
        out = static_cast<SpellingData*>(data[i]);
        found = out->spellingMatch(inSAL, inNAnchors, outPos);
    }
    if(!found) out = NULL;
    return out;
}
