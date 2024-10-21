#include "blinky.h"
#include "../MapElements/tile.h"

bool Blinky::canMove(DirVectors dir_vec)
{
    if(direction == NONE) return true;
    int x = pos().x();
    int y = pos().y();
    auto [x_test, y_test] = dir_vec * SPEED_CO;
    setPos(x + x_test, y + y_test);
    QList<QGraphicsItem*> collisions = collidingItems();

    for (auto item : collisions)
    {
        Tile* tile = dynamic_cast<Tile*>(item);
        if (tile && tile->getType() == WALL)
        {
            setPos(x, y);
            return false;
        }
    }
    setPos(x, y);
    return true;
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
