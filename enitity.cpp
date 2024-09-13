#include "entity.h"

Entity::Entity(const int size) : size(size), direction(NONE), next_direction(NONE), originalPixmap(this->pixmap()) {}

bool Entity::canChangeDir()
{
    if(next_direction == NONE || (next_direction != NONE && !canMove(dir_vec[next_direction])))
    {
        return false;
    }
    setDir(next_direction);
    next_direction = NONE;
    return true;
}

bool Entity::setDir(moveDirections dir)
{
    if(dir == NONE || (dir != NONE &&!canMove(dir_vec[dir]))) return false;
    direction = dir;
    rotateEntity(rotations.at(dir));
    return true;
}

void Entity::move()
{
    if(direction == NONE) return;
    auto [x_v, y_v] = dir_vec[direction] * SPEED_CO;
    int x = pos().x();
    int y = pos().y();
    if(canMove(dir_vec[direction]))
    {
        setPos(x + x_v, y + y_v);
    }
}
