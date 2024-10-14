#include "ghost.h"

bool Ghost::canMove(DirVectors dir_vec)
{
    qDebug() << "zrób to";
    return false;
}

void Ghost::rotateEntity(qreal angle)
{
    qDebug() << "zrób to";
}

void Ghost::load_maze(const int map[MAP_HEIGHT][MAP_WIDTH])
{
    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            maze[i][j] = map[i][j] == 1 ? 0 : 1;
        }
    }
    int pac_x = pacman_pos.x() / size;
    int pac_y = pacman_pos.y() / size;
    maze[pac_y][pac_x] = 2;
}

moveDirections leftTurn(moveDirections curr)
{
    switch (curr)
    {
    case LEFT:  return DOWN;
    case UP:    return LEFT;
    case RIGHT: return UP;
    case DOWN:  return RIGHT;
    case NONE:  return NONE;
    }
    return NONE;
}

moveDirections rightTurn(moveDirections curr)
{
    switch (curr)
    {
    case LEFT:  return UP;
    case UP:    return RIGHT;
    case RIGHT: return DOWN;
    case DOWN:  return LEFT;
    case NONE:  return NONE;
    }
    return NONE;
}

moveDirections backTurn(moveDirections curr)
{
    switch (curr)
    {
    case LEFT: return RIGHT;
    case RIGHT: return LEFT;
    case UP: return DOWN;
    case DOWN: return UP;
    case NONE: return NONE;
    }
    return NONE;
}
