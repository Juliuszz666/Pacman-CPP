#include "inky.h"

#define INKY_NAME "inky"
#define INKY_FILE_STR_DEFAULT ":/img/inkyleft.png"

Inky::Inky(const int size,
           const std::pair<int, int> ini_pos,
           const std::pair<int,int> gate_pos) :
    Ghost(size, ini_pos, INKY_FILE_STR_DEFAULT, gate_pos, INKY_NAME)
{
    direction = LEFT;
}

constexpr int RANDOM_BACK_CHANCE_CO = 420;

void Inky::stickLeft()
{
    if(canMove(dir_vec[leftTurn(direction)]))
    {
        setDir(leftTurn(direction));
    }
    else if (canMove(dir_vec[direction]))
    {
        if(std::rand() % RANDOM_BACK_CHANCE_CO == 0 ) setDir(backTurn(direction));
        else return;
    }
    else if (canMove(dir_vec[rightTurn(direction)]))
    {
        setDir(rightTurn(direction));
    }
    else if (canMove(dir_vec[backTurn(direction)]))
    {
        setDir(backTurn(direction));
    }
}

void Inky::move()
{
    stickLeft();
    auto [x_v, y_v] = dir_vec[direction] * SPEED_CO;
    int x = pos().x();
    int y = pos().y();
    if(canMove(dir_vec[direction]))
    {
        setPos(x + x_v, y + y_v);
    }
}
