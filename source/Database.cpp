#include "Database.h"

#include <fstream>

#include <iostream>
using namespace std;

Database::Database(int inMax) {
    max_data = inMax;
    n_data = 0;
    data = new DatabaseType*[inMax];
    for(int i = 0; i < inMax; i++) data[i] = NULL;
    current_data = NULL;
}

Database::~Database() {
    for(int i = 0; i < max_data; i++) {
        if(data[i]) delete data[i];
    }
    delete [] data;
}

int Database::load(const char* inFilename) {
    int out = 0;
    ifstream myFile(inFilename);
    if(myFile.good()) {
        while(!myFile.eof()) {
            char this_line[MAX_LINE_LENGTH];
            myFile.getline(this_line, MAX_LINE_LENGTH);
            if(this_line[0]!='#') {
                addData(this_line);
                out++;
            }
        }
    } else {
        out = -1;
    }
    return out;
}

void Database::print() {
    printTitle();
    cout << endl;
    for(int i = 0; i < n_data; i++) {
        char thisString[MAX_LINE_LENGTH];
        Data* thisData = data[i];
        data[i]->toString(thisString);
        printf("%s",thisString);
        cout << endl;
    }
}

int Database::getN() const {
    return n_data;
}
