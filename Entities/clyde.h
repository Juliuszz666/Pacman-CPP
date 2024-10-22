#ifndef CLYDE_H
#define CLYDE_H
#include "ghost.h"

class Clyde : public Ghost
{
    void randomLikeMovement();

public:
    Clyde(const int size, const std::pair<int, int> ini_pos, const std::pair<int, int> gate_pos);
    ~Clyde() = default;
    void move() override;
};

#endif // CLYDE_H
