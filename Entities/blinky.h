#ifndef BLINKY_H
#define BLINKY_H

#include "ghost.h"

class Blinky : public Ghost
{
private:
    void stickRight();
public:
    Blinky(const int size, const std::pair<int, int> ini_pos) :
        Ghost(size, ini_pos, ":/img/blinky.png") {direction = UP;}
    ~Blinky() = default;
    void move() override;
    bool canMove(DirVectors dir_vec) override;
};

#endif // BLINKY_H
