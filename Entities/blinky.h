#ifndef BLINKY_H
#define BLINKY_H

#include "ghost.h"

class Blinky : public Ghost
{    
public:
    Blinky(const int size, const std::pair<int, int> ini_pos) :
        Ghost(size, ini_pos, ":/img/blinky.png") {}
    ~Blinky() override = default;
    void move() override;
    void returnToSpawn() override;
};

#endif // BLINKY_H
