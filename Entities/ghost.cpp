#include "ghost.h"
#include "../MapElements/tile.h"
#include <QTimer>

bool Ghost::canMove(DirVectors dir_vec)
{
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

void Ghost::rotateEntity(qreal angle)
{
    qDebug() << "zrób to";
}

void Ghost::returnToSpawn()
{
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
