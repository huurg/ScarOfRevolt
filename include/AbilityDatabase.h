#ifndef ABILITY_DATABASE_HEADER
#define ABILITY_DATABASE_HEADER

#include "Database.h"
#include "AbilityData.h"

class AbilityDatabase: public Database {
    private:
        void addData(const char* inLine);
        void printTitle();
    public:
        AbilityDatabase(int inMax);
};

#endif // ABILITY_DATABASE_HEADER
