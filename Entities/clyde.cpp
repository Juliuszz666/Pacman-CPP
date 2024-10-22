#include "clyde.h"

Clyde::Clyde(const int size,
             const std::pair<int, int> ini_pos,
             const std::pair<int, int> gate_pos) :
    Ghost(size, ini_pos, ":/img/clyde.png", gate_pos)
{
    direction = RIGHT;
}

void Clyde::randomLikeMovement()
{
    if (canMove(dir_vec[leftTurn(direction)]))
    {
        setDir(leftTurn(direction));
    }
    else if (canMove(dir_vec[rightTurn(direction)]))
    {
        if (std::rand() % 3 == 0)
            setDir(rightTurn(direction));
    }
    else if (canMove(dir_vec[direction]))
    {
        if (std::rand() % 69 == 0)
            backTurn(direction);
        return;
    }
    else if (canMove(dir_vec[backTurn(direction)]))
    {
        setDir(backTurn(direction));
    }
}

void Clyde::move()
{
    randomLikeMovement();
    auto [x_v, y_v] = dir_vec[direction] * SPEED_CO;
    int x = pos().x();
    int y = pos().y();
    if (canMove(dir_vec[direction]))
    {
        setPos(x + x_v, y + y_v);
    }
}
