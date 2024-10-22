#ifndef BLINKY_H
#define BLINKY_H

#include "ghost.h"

class Blinky : public Ghost
{
private:
    void stickRight();

public:
    Blinky(const int size, const std::pair<int, int> ini_pos, const std::pair<int, int> gate_pos);
    ~Blinky() = default;
    void move() override;
};

#endif // BLINKY_H
