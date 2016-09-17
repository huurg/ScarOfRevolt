#ifndef SPELLING_DATABASE_HEADER
#define SPELLING_DATABASE_HEADER

#include "Database.h"
#include "SpellingData.h"

class SpellingDatabase: public Database {
    private:
        void addData(const char* inLine);
        void printTitle();
    public:
        SpellingDatabase(int inMax);
        const SpellingData* searchSpelling(const int* inSAL, int inNAnchors, int& outPos);
};

#endif // SPELLING_DATABASE_HEADER
