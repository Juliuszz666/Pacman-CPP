#ifndef PINKY_H
#define PINKY_H
#include "ghost.h"

class Pinky : public Ghost
{
public:
    Pinky(const int size, const std::pair<int, int> ini_pos) :
        Ghost(size, ini_pos, ":/img/pinky.png") {}
    ~Pinky() override = default;
    void move() override;
    void returnToSpawn() override;
};

#endif // PINKY_H
