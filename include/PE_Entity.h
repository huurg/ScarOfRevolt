#ifndef PE_ENTITY_HEADER
#define PE_ENTITY_HEADER

class PE_Entity {
    public:
        enum EntityType {
            ENTITY_TYPE_ENTITY,
            ENTITY_TYPE_CREATOR,
            ENTITY_TYPE_CIRCLE,
            ENTITY_TYPE_WALL
        };
    private:
        EntityType* entityTypes;
        int n_types;
        int max_types;
        static const int DEFAULT_MAX_TYPES = 4;
    protected:
        void addEntityType(EntityType inET);
    public:
        PE_Entity();
        ~PE_Entity();
        void step();
        bool isA(EntityType inET) const;
};

#endif // PE_ENTITY_HEADER
