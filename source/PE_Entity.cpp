#include "PE_Entity.h"

#include <iostream>
using namespace std;

PE_Entity::PE_Entity() {
    max_types = DEFAULT_MAX_TYPES;
    entityTypes = new EntityType[max_types];
    n_types = 0;
    addEntityType(ENTITY_TYPE_ENTITY);
}

PE_Entity::~PE_Entity() {
    delete [] entityTypes;
}

void PE_Entity::step() {
    ;
}

bool PE_Entity::isA(EntityType inET) const {
    bool out = false;
    for(int i = 0; i < n_types; i++) {
        out = (inET==entityTypes[i]);
    }
    return out;
}

void PE_Entity::addEntityType(EntityType inET) {
    if(n_types<max_types) {
        entityTypes[n_types] = inET;
        n_types++;
    } else {
        cout << "No room for adding type" << endl;
    }
}
