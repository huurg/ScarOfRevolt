#include "SpellingData.h"

#include "Data.h"
#include <string>
#include <stdio.h>
#include <cstring>
#include <cstdlib>

#include <iostream>
using namespace std;

SpellingData::SpellingData(int inIndex, int inAbilityIndex, int inAnchorIndex, const char* inSpelling) {
    index = inIndex;
    ability_index = inAbilityIndex;
    anchor_index = inAnchorIndex;
    spelling_string = new string(inSpelling);
    spelling_anchor_list = new int[MAX_ANCHORS];
    n_anchors = 0;

    char thisSpelling[SPELLING_MAX_LENGTH];
    char* thisToken = NULL;
    int this_anchor_index = 0;
    strcpy(thisSpelling, spelling_string->c_str());
    thisToken = strtok(thisSpelling, ANCHOR_DELIMITERS);
    while(thisToken && (n_anchors < MAX_ANCHORS)) {
        this_anchor_index = atoi(thisToken);
        if((this_anchor_index > 0) && (n_anchors < MAX_ANCHORS)) {
            spelling_anchor_list[n_anchors] = this_anchor_index;
            n_anchors++;
        }
        thisToken = strtok(NULL, ANCHOR_DELIMITERS);
    }
}

SpellingData::SpellingData(const SpellingData& inD) {
    index = inD.index;
    ability_index = inD.ability_index;
    anchor_index = inD.anchor_index;
    spelling_string = new string(*(inD.spelling_string));
    n_anchors = inD.n_anchors;
    spelling_anchor_list = new int[n_anchors];
    for(int i = 0; i < n_anchors; i++) {
        spelling_anchor_list[i] = inD.spelling_anchor_list[i];
    }
}

SpellingData::~SpellingData() {
    delete spelling_string;
    delete [] spelling_anchor_list;
}

void SpellingData::toString(char* inDest) {
    sprintf(inDest, "%20d%20d%20d%20s%20d", index, ability_index, anchor_index, spelling_string->c_str(), n_anchors);
}

SpellingData& SpellingData::operator=(const SpellingData& inD) {
    index = inD.index;
    ability_index = inD.ability_index;
    anchor_index = inD.anchor_index;
    spelling_string->assign(*(inD.spelling_string));
    if(n_anchors!=inD.n_anchors) {
        delete [] spelling_anchor_list;
        spelling_anchor_list = new int[inD.n_anchors];
        n_anchors = inD.n_anchors;
    }
    for(int i = 0; i < n_anchors; i++) {
        spelling_anchor_list[i] = inD.spelling_anchor_list[i];
    }
}

bool SpellingData::spellingMatch(const int* inSAL, int inNAnchors, int& outPos) {
    bool out = (n_anchors<=inNAnchors);

    if(out) {
        bool found = false;
        for(int j = 0; (!found)&&(j < (inNAnchors-n_anchors+1)); j++) {
            out = true;
            for(int i = 0; out&&(i < n_anchors); i++) {
                int thisInd = i+j;
                out = (spelling_anchor_list[i]==inSAL[thisInd]);
            }
            if(out) {
                found = true;
                outPos = j;
            }
        }
    }

    return out;
}

int SpellingData::getIndex() const {
    return index;
}

int SpellingData::getAbilityIndex() const {
    return ability_index;
}

int SpellingData::getAnchorIndex() const {
    return anchor_index;
}

int SpellingData::getNAnchors() const {
    return n_anchors;
}
