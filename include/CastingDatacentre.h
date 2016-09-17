#ifndef CASTING_DATACENTRE_HEADER
#define CASTING_DATACENTRE_HEADER

#include "AnchorDatabase.h"
#include "AbilityDatabase.h"
#include "SpellingDatabase.h"
#include "CL_AnchorList.h"

#include "AbilityData.h"

#include <iostream>
using namespace std;
#include <string>
#include <vector>

class CastingDatacentre {
    private:
        AnchorDatabase* anchorDatabase;
        AbilityDatabase* abilityDatabase;
        SpellingDatabase* spellingDatabase;

        static const int MAX_FILENAME = 32;
        static const int MAX_COMMAND_LEN = 100;

        static const char DEFAULT_ANCHOR_DATABASE_PATH[MAX_FILENAME];
        static const char DEFAULT_ABILITY_DATABASE_PATH[MAX_FILENAME];
        static const char DEFAULT_SPELLING_DATABASE_PATH[MAX_FILENAME];

        static const int DEFAULT_ANCHOR_DATABASE_MAX = 1000;
        static const int DEFAULT_ABILITY_DATABASE_MAX = 1000;
        static const int DEFAULT_SPELLING_DATABASE_MAX = 1000;

        string* anchorDatabasePath;
        string* abilityDatabasePath;
        string* spellingDatabasePath;

        int anchorDatabaseMax;
        int abilityDatabaseMax;
        int spellingDatabaseMax;


    public:
        CastingDatacentre();
        ~CastingDatacentre();

        void print();
        int parseCommandString(const char* inCommand, int* outFunctionInds);
        const AbilityData& abilityFromSpelling(const int* inSAL, int inNAnchors);
        bool anchorRank(const vector <int> &inAL, int &out, int inStart=0, int* outLinkLength=NULL);
        int anchorListToTypeString(const vector <int> &inAL, string &outTS);
        int compose(vector <int> &inAL, bool inAnOnly=false);
        int separate(vector <int> &inAL);
        int expand(vector <int> &inAL);
        bool expandOnce(vector <int> &inAL);
        void printAnchorList(const vector <int> &inAL);
        void verifyAnchorList(CL_AnchorList& inAL);
};

#endif // CASTING_DATACENTRE_HEADER
