#include "CL_AnchorList.h"

#include <iostream>
using namespace std;
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

CL_AnchorList::CL_AnchorList() {
    anchorList.clear();
    typeString.clear();
    last_token_end_pos = 0;
    verified = true;
}

CL_AnchorList::CL_AnchorList(const char* inAL) {
    CL_AnchorList();
    char* thisCommandCP = new char[strlen(inAL+1)];

    strcpy(thisCommandCP, inAL);

    char* thisToken = NULL;
    thisToken = strtok(thisCommandCP,"-");

    if(thisToken) {
        int thisInt = atoi(thisToken);

        if(thisInt > 0) {
            add(thisInt);
        } else {
            switch(thisToken[0]) {
                case '(':
                    add(-1);
                    break;
                case ')':
                    add(-2);
                    break;
                case ',':
                    add(-3);
                    break;
                default:
                    cout << "Bad command string! " << thisInt << ", " << thisToken << endl;
                    break;
            }
        }
    }
    while((thisToken=strtok(NULL,"-"))) {
        int thisInt = atoi(thisToken);
        if(thisInt > 0) {
            add(thisInt);
        } else {
            switch(thisToken[0]) {
                case '(':
                    add(-1);
                    break;
                case ')':
                    add(-2);
                    break;
                case ',':
                    add(-3);
                    break;
                default:
                    cout << "Bad command string! Rest" << endl;
                    break;
            }
        }
    }
    delete [] thisCommandCP;
}

CL_AnchorList::~CL_AnchorList() {
    ;
}

CL_AnchorList& CL_AnchorList::add(int inA) {
    anchorList.push_back(inA);
    verified = false;
    return (*this);
}

CL_AnchorList& CL_AnchorList::add(const CL_AnchorList& inAL) {
    int N = inAL.anchorList.size();
    for(int i = 0; i < N; i++) anchorList.push_back(inAL.anchorList.at(i));
    verified = false;
    return (*this);
}

CL_AnchorList& CL_AnchorList::clear() {
    anchorList.clear();
    typeString.clear();
    last_token_end_pos = 0;
    verified = true;
    return (*this);
}

CL_AnchorList& CL_AnchorList::assign(const CL_AnchorList& inAL) {
    clear();
    add(inAL);
    typeString.assign(inAL.typeString);
    last_token_end_pos = inAL.last_token_end_pos;
    verified = inAL.verified;
    return (*this);
}

int CL_AnchorList::getAnchorAt(int i) const {
    int out;
    if(i<anchorList.size()) {
        out = anchorList.at(i);
    } else {
        out = -69;
    }
    return out;
}

char CL_AnchorList::getTypeAt(int i) const {
    char out;
    if(i<typeString.length()) {
        out = typeString.at(i);
    } else {
        out = 'n';
    }
    return out;
}

bool CL_AnchorList::token(CL_AnchorList& outAL, bool reset) {
    bool out = false;
    int thisAnchor;
    if(reset) last_token_end_pos = 0;
    int i = last_token_end_pos;
    CL_AnchorList tempAL;
    while((!out)&&(i<anchorList.size())) {
        thisAnchor = getAnchorAt(i);
        if((thisAnchor==-3)||(i==(anchorList.size()-1))) {
            out = true;
        }
        if(thisAnchor!=-3) {
            tempAL.add(thisAnchor);
        }
        i++;
    }
    last_token_end_pos = i;
    outAL.assign(tempAL);
    return out;
}

void CL_AnchorList::print() const {
    cout << "CL_AnchorList object ";
    if(verified) {
        cout << "(verified): ";
    } else {
        cout << "(unverified): ";
    }
    for(int i = 0; i < getNAnchors(); i++) {
        cout << getAnchorAt(i);
        if(i!=(getNAnchors()-1)) cout << "|";
    }
    if(verified) {
        cout << "; ";
        cout << typeString;
    }
    cout << endl;
}

int CL_AnchorList::getNAnchors() const {
    return anchorList.size();
}
