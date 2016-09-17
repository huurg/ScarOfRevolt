#include "AbilityDatabase.h"

#include <cstring>
#include <cstdlib>
#include <stdio.h>

#include <iostream>
using namespace std;

#include "AbilityData.h"

AbilityDatabase::AbilityDatabase(int inMax): Database(inMax) {
    ;
}

void AbilityDatabase::addData(const char* inLine) {
    char thisLine[MAX_LINE_LENGTH];
    strcpy(thisLine, inLine);
    char* thisToken = NULL;

    int thisIndex = -1;
    char thisName[32];
    int thisFunctionIndex = -1;

    int nargs = 0;

    thisToken = strtok(thisLine,Database::TOKEN_DELIMITERS);
    if(thisToken) {
        thisIndex = atoi(thisToken);
        nargs++;
    }
    thisToken = strtok(NULL,Database::TOKEN_DELIMITERS);
    if(thisToken) {
        strcpy(thisName, thisToken);
        nargs++;
    }
    thisToken = strtok(NULL,Database::TOKEN_DELIMITERS);
    if(thisToken) {
        thisFunctionIndex = atoi(thisToken);
        nargs++;
    }

    if((nargs==3)&&(n_data<max_data)) {
        data[n_data] = new AbilityData(thisIndex,thisName,thisFunctionIndex);
        n_data++;
    }
}

void AbilityDatabase::printTitle() {
    printf("%20s%20s%20s", "Index", "Name", "Function_Index");
}
