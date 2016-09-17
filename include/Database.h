#ifndef DATABASE_HEADER
#define DATABASE_HEADER

#include "Data.h"

#include <iostream>
using namespace std;

class Database {
    protected:
        typedef Data DatabaseType;
        const int MAX_LINE_LENGTH = 1000;
        const char TOKEN_DELIMITERS[16] = ",\t";
        DatabaseType** data;
        DatabaseType* current_data;
        int n_data;
        int max_data;
        virtual void addData(const char* inLine){;}
        virtual void printTitle(){;}
    public:
        Database(int inMax);
        ~Database();
        int load(const char* inFilename);
        void print();
        int getN() const;
};

#endif // DATABASE_HEADER
