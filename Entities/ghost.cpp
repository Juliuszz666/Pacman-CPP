#include "ghost.h"
#include "../MapElements/tile.h"
#include <QTimer>

bool Ghost::canMove(DirVectors dir_vec)
{
    qDebug() << "zrób to";
    return false;
}

void Ghost::rotateEntity(qreal angle)
{
    qDebug() << "zrób to";
}

void Ghost::returnToSpawn()
{

    QTimer* reset_timer = new QTimer(nullptr);
    reset_timer->start(RESET_TIME);
    setPos(spawn_pos.second * size, spawn_pos.first * size);
    setDir(UP);
    connect(reset_timer, &QTimer::timeout, reset_timer, &QTimer::stop);
    connect(reset_timer, &QTimer::destroyed, reset_timer, &QTimer::deleteLater);
    connect(reset_timer, &QTimer::timeout, this, [&]()
    {
                state = INEDIBLE;
    });
}

bool Ghost::noSpawnGate()
{
    QList<QGraphicsItem*> collisions = collidingItems();
    for(auto item : collisions)
    {
        Tile* check = dynamic_cast<Tile*>(item);
        if(check && check->getType() == GHOST_GATE) return false;
    }
    return true;
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