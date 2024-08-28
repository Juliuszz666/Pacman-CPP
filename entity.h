#ifndef ENTITY_H
#define ENTITY_H

class Entity
{

public:
    Entity() = default;
    virtual void move() = 0;
};

#endif // ENTITY_H
