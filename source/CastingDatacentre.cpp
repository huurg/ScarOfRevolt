#include "CastingDatacentre.h"

#include "AnchorDatabase.h"
#include "AbilityDatabase.h"
#include "SpellingDatabase.h"
#include "CL_AnchorList.h"

#include "AbilityData.h"

#include <iostream>
using namespace std;
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>

const char CastingDatacentre::DEFAULT_ANCHOR_DATABASE_PATH[] = "anchors.txt";
const char CastingDatacentre::DEFAULT_ABILITY_DATABASE_PATH[] = "abilities.txt";
const char CastingDatacentre::DEFAULT_SPELLING_DATABASE_PATH[] = "spellings.txt";

CastingDatacentre::CastingDatacentre() {
    anchorDatabasePath = new string(DEFAULT_ANCHOR_DATABASE_PATH);
    abilityDatabasePath = new string(DEFAULT_ABILITY_DATABASE_PATH);
    spellingDatabasePath = new string(DEFAULT_SPELLING_DATABASE_PATH);

    anchorDatabaseMax = DEFAULT_ANCHOR_DATABASE_MAX;
    abilityDatabaseMax = DEFAULT_ABILITY_DATABASE_MAX;
    spellingDatabaseMax = DEFAULT_SPELLING_DATABASE_MAX;

    anchorDatabase = new AnchorDatabase(anchorDatabaseMax);
    abilityDatabase = new AbilityDatabase(abilityDatabaseMax);
    spellingDatabase = new SpellingDatabase(spellingDatabaseMax);

    anchorDatabase->load(anchorDatabasePath->c_str());
    abilityDatabase->load(abilityDatabasePath->c_str());
    spellingDatabase->load(spellingDatabasePath->c_str());
}

CastingDatacentre::~CastingDatacentre() {
    delete anchorDatabasePath;
    delete abilityDatabasePath;
    delete spellingDatabasePath;
    delete anchorDatabase;
    delete abilityDatabase;
    delete spellingDatabase;
}

void CastingDatacentre::print() {
    cout << "CastingDatacentre object: anchors = " << anchorDatabase->getN() << "; abilities = " << abilityDatabase->getN() << "; spellings = " << spellingDatabase->getN() << "." << endl;
}

int CastingDatacentre::parseCommandString(const char* inCommand, int* outFunctionInds) {
    string thisCommand = inCommand;
    vector<int> thisCommandVI;

    // Stage 0: Convert command to vector of ints
    cout << "Stage 0" << endl;
    char thisCommandCP[MAX_COMMAND_LEN];
    strcpy(thisCommandCP, thisCommand.c_str());
    char* thisToken = NULL;
    thisToken = strtok(thisCommandCP,"-");
    if(thisToken) {
        int thisInt = atoi(thisToken);
        if(thisInt > 0) {
            thisCommandVI.push_back(thisInt);
        } else {
            switch(thisToken[0]) {
                case '(':
                    thisCommandVI.push_back(-1);
                    break;
                case ')':
                    thisCommandVI.push_back(-2);
                    break;
                default:
                    cout << "Bad command string!" << endl;
                    break;
            }
        }
    }
    while((thisToken=strtok(NULL,"-"))) {
        int thisInt = atoi(thisToken);
        if(thisInt > 0) {
            thisCommandVI.push_back(thisInt);
        } else {
            switch(thisToken[0]) {
                case '(':
                    thisCommandVI.push_back(-1);
                    break;
                case ')':
                    thisCommandVI.push_back(-2);
                    break;
                default:
                    cout << "Bad command string!" << endl;
                    break;
            }
        }
    }

    // Stage 1: Convert all occurrences of -1,-2 into 0
    cout << "Stage 1" << endl;
    {
        vector<int> tempVI;
        for(int i = 0; i < thisCommandVI.size(); i++) {
            if((thisCommandVI[i]==-1)&&(i < (thisCommandVI.size()-1))&&(thisCommandVI[i+1]==-2)) {
                tempVI.push_back(0);
                i++;
            } else {
                tempVI.push_back(thisCommandVI[i]);
            }
        }
        thisCommandVI = tempVI;
    }

    // Stage 2: Replace anchor to anchor spellings
    cout << "Stage 2" << endl;
    compose(thisCommandVI,true);
    printAnchorList(thisCommandVI);

    // Stage 2b Strip all 0's
    cout << "Stage 2b" << endl;
    for(int i = 0; i < thisCommandVI.size(); i++) {
        if(thisCommandVI[i]==0) {
            thisCommandVI.erase(thisCommandVI.begin()+i);
        }
    }
    printAnchorList(thisCommandVI);
    // Stage 3 Separate into links:
    cout << "Stage 3" << endl;
    separate(thisCommandVI);
    cout << "Separated:" << endl;
    printAnchorList(thisCommandVI);

    // Stage 4 Expand brackets:
    cout << "Stage 4" << endl;
    expand(thisCommandVI);
    cout << "Expanded:" << endl;
    printAnchorList(thisCommandVI);


    return 0;
}

const AbilityData& CastingDatacentre::abilityFromSpelling(const int* inSAL, int inNAnchors) {
    bool found = false;
    ;
}

bool CastingDatacentre::anchorRank(const vector <int> &inAL, int &out, int inStart, int* outLinkLength) {
    out = 0;
    bool outB = true;
    bool unchanged = true;
    bool prel_link_found = false;
    bool link_found = false;
    int N = inAL.size();
    string thisTS;
    anchorListToTypeString(inAL, thisTS);
    bool in_bracket = false;
    vector <int> bracket_contents;
    for(int i = inStart; (!link_found)&&outB&&(i < N); i++) {
        char thisType = thisTS[i];
        int thisAnchor = inAL[i];
        if(!in_bracket) {
            out += (int)(thisType=='c');
            out -= (int)(thisType=='f');
            if(unchanged&&(out!=0)) unchanged = false;
            if(thisType=='(') {
                in_bracket = true;
                bracket_contents.clear();
            }
        } else {
            if((thisType==')')||(i==(N-1))) {
                in_bracket = false;
                if(thisAnchor>0) bracket_contents.push_back(thisAnchor);
                compose(bracket_contents);
                string bracket_type;
                anchorListToTypeString(bracket_contents,bracket_type);
                int n_cs = 0;
                int n_fs = 0;
                int n_us = 0;
                for(int j = 0; j < bracket_contents.size(); j++) {
                    char thisBracketType = bracket_type[j];
                    n_cs += (int)(thisBracketType=='c');
                    n_fs += (int)(thisBracketType=='f');
                    n_us += (int)(thisBracketType=='u');
                }
                if((n_cs>0)&&(n_fs==0)) {
                    out++;
                    unchanged = false;
                } else if(unchanged) {
                    outB = false;

                }
                if((n_fs>0)&&(n_cs==0)) {
                    out--;
                    unchanged = false;
                }
                if((n_cs>0)&&(n_fs>0)) {
                    outB = false;
                    out = 0;
                }
            } else {
                bracket_contents.push_back(thisAnchor);
            }
        }
        if((!link_found)&&outLinkLength&&(!unchanged)&&(out==0)) {
            link_found = true;
            *outLinkLength = i+1;
        } else if(outLinkLength&&(i==(N-1))) {
            if(out==0) {
                link_found = true;
                *outLinkLength = i+1;
            } else {
                *outLinkLength = -1;
            }
        }
    }

    return outB;
}

int CastingDatacentre::anchorListToTypeString(const vector <int> &inAL, string &outTS) {
    int out = 0;

    outTS.clear();
    int n = inAL.size();
    for(int i = 0; i < n; i++) {
        outTS += anchorDatabase->getTypeFromIndex(inAL[i]);
    }

    return out;
}

int CastingDatacentre::compose(vector <int> &thisCommandVI, bool inAnOnly) {
    bool unchanged = false;
    while(!unchanged) {
        //for(int i = 0; i < thisCommandVI.size(); i++) cout << "[" << i << "] = " << thisCommandVI[i] << endl;
        unchanged = true;
        vector<int> tempVI;
        bool start = false;
        vector<int> tempSub;
        int thisRank = 0;
        for(int i = 0; i < thisCommandVI.size(); i++) {
            if(!start) {
                if(thisCommandVI[i]>0) {
                    start = true;
                    tempSub.clear();
                    tempSub.push_back(thisCommandVI[i]);
                    if(i == (thisCommandVI.size()-1)) {
                        start = false;
                        int subSize = tempSub.size();
                        if(subSize>1) {
                            int* subSpelling = new int[subSize];
                            for(int j = 0; j < subSize; j++) {
                                subSpelling[j] = tempSub[j];
                            }
                            int thisPos;
                            const SpellingData* thisSpellingP1 = spellingDatabase->searchSpelling(subSpelling, subSize, thisPos);

                            if(thisSpellingP1) {
                                SpellingData thisSpelling(*thisSpellingP1);
                                int thisAnchor = thisSpelling.getAnchorIndex();
                                int thisAbility = thisSpelling.getAbilityIndex();
                                bool thisFinal = (thisAbility>0)&&(thisCommandVI.size()==tempSub.size()==thisSpelling.getNAnchors());
                                if((thisAnchor>0)&&(!thisFinal)&&((!inAnOnly)||(inAnOnly&&(thisAbility==0)))) {
                                    unchanged = false;
                                    bool written = false;
                                    for(int j = 0; j < tempSub.size(); j++) {
                                        if((j>=thisPos)&&(j<(thisPos+thisSpelling.getNAnchors()))) {
                                            if(!written) {
                                                tempVI.push_back(thisAnchor);
                                                written = true;
                                            }
                                        } else {
                                            tempVI.push_back(tempSub[j]);
                                        }
                                    }
                                } else {
                                    for(int j = 0; j < tempSub.size(); j++) tempVI.push_back(tempSub[j]);
                                }
                            } else {
                                for(int j = 0; j < tempSub.size(); j++) tempVI.push_back(tempSub[j]);
                            }
                            delete [] subSpelling;
                        } else {
                            tempVI.push_back(tempSub[0]);
                        }
                    }
                } else {
                    tempVI.push_back(thisCommandVI[i]);
                }
            } else {
                if(thisCommandVI[i]>0) {
                    tempSub.push_back(thisCommandVI[i]);
                    if(i == (thisCommandVI.size()-1)) {
                        start = false;
                        int subSize = tempSub.size();
                        int* subSpelling = new int[subSize];
                        for(int j = 0; j < subSize; j++) {
                            subSpelling[j] = tempSub[j];
                        }
                        int thisPos;
                        const SpellingData* thisSpellingP2 = spellingDatabase->searchSpelling(subSpelling, subSize, thisPos);
                        if(thisSpellingP2) {
                            SpellingData thisSpelling(*thisSpellingP2);
                            int thisAnchor = thisSpelling.getAnchorIndex();
                            int thisAbility = thisSpelling.getAbilityIndex();
                            bool thisFinal = (thisAbility>0)&&(thisCommandVI.size()==tempSub.size()==thisSpelling.getNAnchors());
                            if((thisAnchor>0)&&(!thisFinal)&&((!inAnOnly)||(inAnOnly&&(thisAbility==0)))) {
                                unchanged = false;
                                bool written = false;
                                for(int j = 0; j < tempSub.size(); j++) {
                                    if((j>=thisPos)&&(j<(thisPos+thisSpelling.getNAnchors()))) {
                                        if(!written) {
                                            tempVI.push_back(thisAnchor);
                                            written = true;
                                        }
                                    } else {
                                        tempVI.push_back(tempSub[j]);
                                    }
                                }
                            } else {
                                for(int j = 0; j < tempSub.size(); j++) tempVI.push_back(tempSub[j]);
                            }
                        } else {
                            for(int j = 0; j < tempSub.size(); j++) tempVI.push_back(tempSub[j]);
                        }
                        delete [] subSpelling;
                    }
                } else {
                    start = false;
                    int subSize = tempSub.size();
                    if(subSize>1) {
                        int* subSpelling = new int[subSize];
                        for(int j = 0; j < subSize; j++) {
                            subSpelling[j] = tempSub[j];
                        }
                        int thisPos;
                        const SpellingData* thisSpellingP3 = spellingDatabase->searchSpelling(subSpelling, subSize,thisPos);
                        if(thisSpellingP3) {
                            SpellingData thisSpelling(*thisSpellingP3);
                            int thisAnchor = thisSpelling.getAnchorIndex();
                            int thisAbility = thisSpelling.getAbilityIndex();
                            bool thisFinal = (thisAbility>0)&&(thisCommandVI.size()==tempSub.size()==thisSpelling.getNAnchors());
                            if((thisAnchor>0)&&(!thisFinal)&&((!inAnOnly)||(inAnOnly&&(thisAbility==0)))) {
                                unchanged = false;
                                bool written = false;
                                for(int j = 0; j < tempSub.size(); j++) {
                                    if((j>=thisPos)&&(j<(thisPos+thisSpelling.getNAnchors()))) {
                                        if(!written) {
                                            tempVI.push_back(thisAnchor);
                                            written = true;
                                        }
                                    } else {
                                        tempVI.push_back(tempSub[j]);
                                    }
                                }
                            } else {
                                for(int j = 0; j < tempSub.size(); j++) tempVI.push_back(tempSub[j]);
                            }
                        } else {
                            for(int j = 0; j < tempSub.size(); j++) tempVI.push_back(tempSub[j]);
                        }
                        delete [] subSpelling;
                    } else {
                        if(subSize == 1) tempVI.push_back(tempSub[0]);
                    }
                    tempVI.push_back(thisCommandVI[i]);
                }
            }
        }
        thisCommandVI = tempVI;
        //cout << endl;
    }
    return 0;
}

int CastingDatacentre::separate(vector <int> &inAL) {

    vector <int> tempAL;
    bool success;
    int thisRank;
    int thisLength = 0;
    int prevLength;
    int mycount = 0;
    vector <int> thisAL;

    do {
        thisAL.clear();
        prevLength = thisLength;
        success = anchorRank(inAL,thisRank,prevLength,&thisLength);
        success = success&&(thisLength>0)&&(thisLength<inAL.size());
        if(success) {
            for(int i = prevLength; i < thisLength; i++) {
                thisAL.push_back(inAL[i]);
            }
            compose(thisAL);


            for(int i = 0; i < thisAL.size(); i++) tempAL.push_back(thisAL[i]);
            if(thisLength<inAL.size()) {
                tempAL.push_back(-3);
            }
        } else {
            for(int i = prevLength; i < inAL.size(); i++) {
                thisAL.push_back(inAL[i]);
            }
            compose(thisAL);
            for(int i = 0; i < thisAL.size(); i++) tempAL.push_back(thisAL[i]);
        }
        mycount++;
    } while(success);
    inAL = tempAL;
    return 0;
}

bool CastingDatacentre::expandOnce(vector <int> &inAL) {
    vector <int> preAL, thisAL, postAL, thisA, thisB, thisC, thisExpansion, thisSub;
    string inTS;
    anchorListToTypeString(inAL, inTS);

    bool unchanged = true;

    int state = 0;
    preAL.clear(); thisAL.clear(); postAL.clear(); thisA.clear(); thisB.clear(); thisC.clear(); thisExpansion.clear(); thisSub.clear();

    bool brackets = false;
    for(int i = 0; i < inAL.size(); i++) {
        int thisAnchor = inAL[i];
        char thisType = inTS[i];
        if(state==0) {
            if((thisAnchor==-3)||(i==(inAL.size()-1))) {
                if(brackets) {
                    state = 1;
                    if((i==(inAL.size()-1))&&(thisAnchor!=-3)) thisAL.push_back(thisAnchor);
                } else {
                    if((i==(inAL.size()-1))&&(thisAnchor!=-3)) thisAL.push_back(thisAnchor);
                    for(int j = 0; j < thisAL.size(); j++) preAL.push_back(thisAL[j]);
                    if(!(i==(inAL.size()-1))) preAL.push_back(-3);
                    thisAL.clear();
                }
            } else {
                if(thisAnchor==-1) brackets = true;
                thisAL.push_back(thisAnchor);
            }
        } else if(state==1) {
            postAL.push_back(thisAnchor);
        }
    }
    unchanged = !brackets;
    if(unchanged) return !unchanged;

    state = 0;
    string thisTS;
    anchorListToTypeString(thisAL,thisTS);

    int n_cs = 0, n_fs = 0, n_us = 0;
    for(int i = 0; i < thisAL.size(); i++) {
        int thisAnchor = thisAL[i];
        char thisType = thisTS[i];
        if(state==0) {
            if(thisAnchor==-1) {
                state=1;
            } else {
                thisA.push_back(thisAnchor);
            }
        } else if(state==1) {
            if(thisAnchor==-2) {
                state=2;
            } else {
                thisB.push_back(thisAnchor);
                n_cs += (int)(thisType=='c');
                n_fs += (int)(thisType=='f');
                n_us += (int)(thisType=='u');
            }
        } else if(state==2) {
            thisC.push_back(thisAnchor);
        }
    }
    state = 0;
    for(int i = 0; i < thisB.size(); i++) {
        int thisAnchor = thisB[i];
        char thisType = anchorDatabase->getTypeFromIndex(thisAnchor);
        if((n_cs>0)&&(n_fs==0)) {
            if(state==0) {
                if(thisType=='c') {
                    state = 1;
                    thisSub.push_back(thisAnchor);
                }
            } else if(state==1) {
                if(thisType=='u') {
                    thisSub.push_back(thisAnchor);
                    if(i==(thisB.size()-1)) {
                        for(int j = 0; j < thisA.size(); j++) thisExpansion.push_back(thisA[j]);
                        for(int j = 0; j < thisSub.size(); j++) thisExpansion.push_back(thisSub[j]);
                        for(int j = 0; j < thisC.size(); j++) thisExpansion.push_back(thisC[j]);
                        if(!((i==(thisB.size()-1))&&(!postAL.size())))thisExpansion.push_back(-3);
                    }
                } else if(thisType=='c') {
                    for(int j = 0; j < thisA.size(); j++) thisExpansion.push_back(thisA[j]);
                    for(int j = 0; j < thisSub.size(); j++) thisExpansion.push_back(thisSub[j]);
                    for(int j = 0; j < thisC.size(); j++) thisExpansion.push_back(thisC[j]);
                    if(!((i==(thisB.size()-1))&&(!postAL.size())))thisExpansion.push_back(-3);
                    thisSub.clear();
                    thisSub.push_back(thisAnchor);
                }
                if((thisType=='c')&&(i==(thisB.size()-1))) {
                    thisExpansion.push_back(-3);
                    for(int j = 0; j < thisA.size(); j++) thisExpansion.push_back(thisA[j]);
                    for(int j = 0; j < thisSub.size(); j++) thisExpansion.push_back(thisSub[j]);
                    for(int j = 0; j < thisC.size(); j++) thisExpansion.push_back(thisC[j]);
                    if(!((i==(thisB.size()-1))&&(!postAL.size())))thisExpansion.push_back(-3);
                }
            }
        } else if((n_fs>0)&&(n_cs==0)) {
            if((thisType=='u')&&(i < (thisB.size()-1))) {
                thisSub.push_back(thisAnchor);
            } else if(thisType=='f') {
                thisSub.push_back(thisAnchor);
                for(int j = 0; j < thisA.size(); j++) thisExpansion.push_back(thisA[j]);
                for(int j = 0; j < thisSub.size(); j++) thisExpansion.push_back(thisSub[j]);
                for(int j = 0; j < thisC.size(); j++) thisExpansion.push_back(thisC[j]);
                if(!((i==(thisB.size()-1))&&(!postAL.size())))thisExpansion.push_back(-3);
                thisSub.clear();
            }
        } else if((n_fs==0)&&(n_cs==0)) {
            for(int j = 0; j < thisA.size(); j++) thisExpansion.push_back(thisA[j]);
            thisExpansion.push_back(thisAnchor);
            for(int j = 0; j < thisC.size(); j++) thisExpansion.push_back(thisC[j]);
            if(!((i==(thisB.size()-1))&&(!postAL.size())))thisExpansion.push_back(-3);
        }
    }
    inAL.clear();
    inAL=preAL;
    //if(preAL.size()) inAL.push_back(-3);
    for(int i = 0; i < thisExpansion.size(); i++) {
        inAL.push_back(thisExpansion[i]);
    }
    for(int i = 0; i < postAL.size(); i++) {
        inAL.push_back(postAL[i]);
    }

    cout << "preAL:" << endl;
    printAnchorList(preAL);
    cout << "thisAL:" << endl;
    printAnchorList(thisAL);
    cout << "postAL:" << endl;
    printAnchorList(postAL);
    cout << "out:" << endl;
    printAnchorList(inAL);

    return (!unchanged);
}

void CastingDatacentre::printAnchorList(const vector <int> &inAL) {
    string myString;
    anchorListToTypeString(inAL, myString);
    cout << myString << endl;
    for(int i = 0; i < inAL.size(); i++) cout << "[" << i << "] = " << inAL[i] << endl;
}

int CastingDatacentre::expand(vector <int> &inAL) {
    bool changed;
    int i = -1;
    do {
        i++;
        changed = expandOnce(inAL);
        //cout << "i = " << i << ", changed = " << changed <<  endl;
    } while(changed);
    return 0;
}

void CastingDatacentre::verifyAnchorList(CL_AnchorList& inAL) {
    if(!inAL.verified) {
        inAL.typeString.clear();
        for(int i = 0; i < inAL.getNAnchors(); i++) {
            int thisAnchor = inAL.getAnchorAt(i);
            char thisType = anchorDatabase->getTypeFromIndex(thisAnchor);
            inAL.typeString += thisType;
        }
        inAL.verified = true;
    }
}
