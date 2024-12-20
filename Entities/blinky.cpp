#include "blinky.h"

#define BLINKY_NAME "blinky"
#define BLINKY_FILE_STR_DEFAULT ":/img/blinkyup.png"

Blinky::Blinky(const int size,
               const std::pair<int, int> ini_pos,
               const std::pair<int, int> gate_pos) :
    Ghost(size, ini_pos, BLINKY_FILE_STR_DEFAULT, gate_pos, BLINKY_NAME)
{
    direction = UP;
}

void Blinky::stickRight()
{
    if (canMove(dir_vec[direction]))
    {
        return;
    }
    else if(canMove(dir_vec[rightTurn(direction)]))
    {
        setDir(rightTurn(direction));
    }
    else if (canMove(dir_vec[leftTurn(direction)]))
    {
        setDir(leftTurn(direction));
    }
    else if (canMove(dir_vec[backTurn(direction)]))
    {
        setDir(backTurn(direction));
    }
}

void Blinky::move()
{
    stickRight();
    auto [x_v, y_v] = dir_vec[direction] * SPEED_CO;
    int x = pos().x();
    int y = pos().y();
    if(canMove(dir_vec[direction]))
    {
        setPos(x + x_v, y + y_v);
    }
}
