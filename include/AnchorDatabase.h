#ifndef ANCHOR_DATABASE_HEADER
#define ANCHOR_DATABASE_HEADER

#include "Database.h"
#include "AnchorData.h"

class AnchorDatabase: public Database {
    protected:
        void addData(const char* inLine);
        void printTitle();
    public:
        AnchorDatabase(int inMax);
        char getTypeFromIndex(int inIndex);
};

#endif // ANCHOR_DATABASE_HEADER
