#ifndef CLYDE_H
#define CLYDE_H
#include "ghost.h"

class Clyde : public Ghost
{
public:
    Clyde(const int size, const std::pair<int, int> ini_pos) :
        Ghost(size, ini_pos, ":/img/clyde.png") {}
    ~Clyde() = default;
    void move() override;
};

#endif // CLYDE_H
