#include "PE_Creator.h"

#include "PE_PhysicsEnvironment.h"

#include <iostream>
using namespace std;

PE_Creator::PE_Creator() {
    addEntityType(ENTITY_TYPE_CREATOR);

    create_zero_entity_index = 0;
    create_n_entities = 0;
    create_max_n_entities = DEFAULT_MAX_N_ENTITIES;
    destroy_zero_entity_index = 0;
    destroy_n_entities = 0;
    destroy_max_n_entities = DEFAULT_MAX_N_ENTITIES;

    entity_creation_queue = new PE_Entity*[create_max_n_entities];
    for(int i = 0; i < create_max_n_entities; i++) entity_creation_queue[i] = NULL;
    entity_deletion_queue = new int[destroy_max_n_entities];
    for(int i = 0; i < destroy_max_n_entities; i++) entity_deletion_queue[i] = -1;
}

PE_Creator::~PE_Creator() {
    delete [] entity_creation_queue;
    delete [] entity_deletion_queue;
}

bool PE_Creator::createEntity(PE_Entity* inE) {
    bool out = false;
    if(create_n_entities<create_max_n_entities) {
        int thisIndex = (create_zero_entity_index+create_n_entities)%create_max_n_entities;
        entity_creation_queue[thisIndex] = inE;
        create_n_entities++;
        create_n_entities %= create_max_n_entities;
        out = true;
        cout << "Entity created. n_entities = " << create_n_entities << endl;
    }
    return out;
}

bool PE_Creator::destroyEntity(int inI) {
    bool out = false;
    if(destroy_n_entities<destroy_max_n_entities) {
        int thisIndex = (destroy_zero_entity_index+destroy_n_entities)%destroy_max_n_entities;
        entity_deletion_queue[thisIndex] = inI;
        destroy_n_entities++;
        destroy_n_entities %= create_max_n_entities;
        out = true;
    }
    return out;
}

PE_Entity* PE_Creator::nextEntityToCreate(int inI) {
    PE_Entity* out = NULL;
    if(create_n_entities>0) {
        int thisI = create_zero_entity_index;
        out = entity_creation_queue[thisI];
        entity_creation_queue[thisI] = NULL;
        create_zero_entity_index++;
        create_n_entities--;
        create_zero_entity_index %= create_max_n_entities;
        cout << "Env received entity" << endl;
    }
    return out;
}

int PE_Creator::nextEntityToDestroy() {
    int out = -1;
    if(destroy_n_entities>0) {
        int thisI = destroy_zero_entity_index;
        out = entity_deletion_queue[thisI];
        entity_deletion_queue[thisI] = NULL;
        destroy_zero_entity_index++;
        destroy_n_entities--;
        destroy_zero_entity_index %= destroy_max_n_entities;
    }
    return out;
}

void PE_Creator::destroyed(int inI) {
    ;
}
