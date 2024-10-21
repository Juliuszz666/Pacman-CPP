#ifndef INKY_H
#define INKY_H
#include "ghost.h"

class Inky : public Ghost
{
private:
    void stickLeft();
public:
    Inky(const int size, const std::pair<int, int> ini_pos) :
        Ghost(size, ini_pos, ":/img/inky.png") {direction = LEFT;}
    ~Inky() = default;
    void move() override;
    bool canMove(DirVectors dir_vec) override;
};

#endif // INKY_H
