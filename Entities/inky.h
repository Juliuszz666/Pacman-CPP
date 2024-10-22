#ifndef INKY_H
#define INKY_H
#include "ghost.h"

class Inky : public Ghost
{
private:
    void stickLeft();
public:
    Inky(const int size, const std::pair<int, int> ini_pos, const std::pair<int,int> gate_pos);
    ~Inky() = default;
    void move() override;
    //bool canMove(DirVectors dir_vec) override;
};

#endif // INKY_H
