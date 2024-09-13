#ifndef GHOST_H
#define GHOST_H
#include "entity.h"

class Ghost : public Entity
{
private:
    GhostState state;

public:
    GhostState getState() {return state;}
    void setState(GhostState newstate) {this->state = newstate;}

    //Ghost();
};

#endif // GHOST_H
