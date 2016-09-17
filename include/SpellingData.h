#ifndef SPELLING_DATA_HEADER
#define SPELLING_DATA_HEADER

#include "Data.h"
#include <string>
#include <iostream>
using namespace std;

class SpellingData: public Data {
    private:
        const int MAX_ANCHORS = 100;
        const char ANCHOR_DELIMITERS[16] = "-";
        const char SPELLING_MAX_LENGTH = 100;
        int index;
        int ability_index;
        int anchor_index;
        string* spelling_string;
        int* spelling_anchor_list;
        int n_anchors;
    public:
        SpellingData(const SpellingData& inD);
        SpellingData(int inIndex, int inAbilityIndex, int inAnchorIndex, const char* inSpelling);
        ~SpellingData();

        void toString(char* inDest);
        SpellingData& operator=(const SpellingData& inD);
        bool spellingMatch(const int* inSAL, int inNAnchors, int& outPos);

        int getIndex() const;
        int getAbilityIndex() const;
        int getAnchorIndex() const;
        int getNAnchors() const;
};

#endif // SPELLING_DATA_HEADER
