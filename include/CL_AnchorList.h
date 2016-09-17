#ifndef CL_ANCHOR_LIST_HEADER
#define CL_ANCHOR_LIST_HEADER

#include <iostream>
using namespace std;
#include <vector>
#include <string>

#ifndef CASTING_DATACENTRE_FWD
#define CASTING_DATACENTRE_FWD
class CastingDatacentre;
#endif // CASTING_DATACENTRE_FWD

class CL_AnchorList {
    friend CastingDatacentre;
    private:
        vector <int> anchorList;
        string typeString;
        int last_token_end_pos;
        bool verified;
    public:
        CL_AnchorList();
        CL_AnchorList(const CL_AnchorList& inAL);
        CL_AnchorList(const char* inAL);
        ~CL_AnchorList();

        CL_AnchorList& add(int inA);
        CL_AnchorList& add(const CL_AnchorList& inAL);
        CL_AnchorList& clear();
        CL_AnchorList& assign(const CL_AnchorList& inAL);

        int getAnchorAt(int i) const;
        char getTypeAt(int i) const;
        int getNAnchors() const;

        bool token(CL_AnchorList& outAL, bool reset=false);

        void print() const;
};

#endif // CL_ANCHOR_LIST_HEADER
