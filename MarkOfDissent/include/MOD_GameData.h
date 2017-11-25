#ifndef MOD_GAMEDATA
#define MOD_GAMEDATA

#include <iostream>
using namespace std;

#include <vector>

class MOD_GameData {
    private:
        // This object's properties
        // Position
        double x, y, z;
        // Velocity
        double vx, vy, vz;
        // Boundary
            /* MOD_Boundary class: The boundary data associated with this GameData for physics purposes */
        // Graphics
            /* MOD_Graphics class: Graphics data for rendering purposes (texture/mesh etc.) */

        // Children
        MOD_GameData* children;
        int max_children;
        int n_children;

    public:

    // Methods
        /* Physics: Performs a single physics time-step on all child GameData objects and their pair-wise interactions */
        /* God powers: Creation and destruction of it's GameData children. ONLY the direct parent can use new/delete to create/destroy children. Must use public functions otherwise.
        /* Evolution: Allows GameData to interact with other sibling GameData. E.g. A fireball in contact with a compatible agent = {enemy/player/monster} will subtract a certain amount of health from the contactee. Subsequently, God will destroy everything with <=0 health.


};

#endif // MOD_GAMEDATA
