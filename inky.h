#ifndef INKY_H
#define INKY_H
#include "ghost.h"

class Inky : public Ghost
{
public:
    Inky(const int size, const std::pair<int, int> ini_pos) :
        Ghost(size, ini_pos, ":/img/inky.png") {}
    ~Inky() override = default;
    void move() override;
    void returnToSpawn() override;
};

#endif // INKY_H
