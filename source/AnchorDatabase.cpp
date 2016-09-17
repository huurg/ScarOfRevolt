#include "AnchorDatabase.h"

#include <cstring>
#include <cstdlib>
#include <stdio.h>

#include <iostream>
using namespace std;

#include "AnchorData.h"

AnchorDatabase::AnchorDatabase(int inMax): Database(inMax) {
    ;
}

void AnchorDatabase::addData(const char* inLine) {
    char thisLine[MAX_LINE_LENGTH];
    strcpy(thisLine, inLine);
    char* thisToken = NULL;

    int thisIndex = -1;
    char thisType = '\0';
    char thisName[32];
    char thisShortname[32];
    bool thisAssignable = false;

    int nargs = 0;

    thisToken = strtok(thisLine,Database::TOKEN_DELIMITERS);
    if(thisToken) {
        thisIndex = atoi(thisToken);
        nargs++;
    }
    thisToken = strtok(NULL,Database::TOKEN_DELIMITERS);
    if(thisToken) {
        thisType = thisToken[0];
        nargs++;
    }
    thisToken = strtok(NULL,Database::TOKEN_DELIMITERS);
    if(thisToken) {
        strcpy(thisName, thisToken);
        nargs++;
    }
    thisToken = strtok(NULL,Database::TOKEN_DELIMITERS);
    if(thisToken) {
        strcpy(thisShortname, thisToken);
        nargs++;
    }
    thisToken = strtok(NULL,Database::TOKEN_DELIMITERS);
    if(thisToken) {
        thisAssignable = (bool)atoi(thisToken);
        nargs++;
    }

    if((nargs==5)&&(n_data<max_data)) {
        data[n_data] = new AnchorData(thisIndex,thisType,thisName,thisShortname,thisAssignable);
        n_data++;
    }
}

void AnchorDatabase::printTitle() {
    printf("%20s%20s%20s%20s%20s", "Index", "Type", "Name", "Shortname","Assignable");
}

char AnchorDatabase::getTypeFromIndex(int inIndex) {
    char out = 'n';
    switch(inIndex) {
    case 0:
        out = '0';
        break;
    case -1:
        out = '(';
        break;
    case -2:
        out = ')';
        break;
    case -3:
        out = ',';
        break;
    default:
        for(int i = 0; i < n_data; i++) {
            AnchorData* thisAnchor = static_cast<AnchorData*>(data[i]);
            if(thisAnchor->getIndex()==inIndex) {
                out = thisAnchor->getType();
            }
        }
        break;
    }

    return out;
}
