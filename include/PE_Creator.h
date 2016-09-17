#ifndef PE_CREATOR_HEADER
#define PE_CREATOR_HEADER

#include "PE_Entity.h"

class PE_Creator: public PE_Entity {
    private:
        PE_Entity** entity_creation_queue;
        int* entity_deletion_queue;
        int create_zero_entity_index;
        int create_n_entities;
        int create_max_n_entities;
        int destroy_zero_entity_index;
        int destroy_n_entities;
        int destroy_max_n_entities;
        static const int DEFAULT_MAX_N_ENTITIES = 10;
    public:
        PE_Creator();
        ~PE_Creator();
        bool createEntity(PE_Entity* inE); // Add inE to creation queue to be added to PE_Environment by PE_Environment (externally)
        bool destroyEntity(int inI);
        PE_Entity* nextEntityToCreate(int inI); // Environment calls this to gain access to the next entity in the creation queue. Environment gives this entity the index, inI, at which the created entity is stored in Environment's entity list.
        int nextEntityToDestroy(); // Environment calls this to access the index of the next entity to destroy.
        void destroyed(int inI); // This method tells this entity that the entity at inI has been destroyed. Derived classes that store created entities should assign their pointers to NULL if destroyed.

};

#endif // PE_CREATOR_HEADER
