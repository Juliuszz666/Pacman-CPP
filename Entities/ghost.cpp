#include "ghost.h"
#include "../MapElements/tile.h"
#include <QTimer>
#include <QPixmap>

Ghost::Ghost(const int size,
             const std::pair<int, int> ini_pos,
             const QString &img_filename,
             const std::pair<int,int> g_pos,
             const QString& name) :
    Entity(size, ini_pos, img_filename),
    state(INEDIBLE),
    out_of_spawn(false),
    spawn_pos(ini_pos),
    reset_timer(new QTimer(nullptr)),
    pacman_pos({}),
    gate_pos(g_pos),
    name(name)
{}

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
    QString dir_str;
    switch (direction) {
    case LEFT:
        dir_str = "left";
        break;
    case UP:
        dir_str = "up";
        break;
    case RIGHT:
        dir_str = "right";
        break;
    case DOWN:
        dir_str = "down";
        break;
    default:
        return;
    }
    setPixmap(QPixmap(":/img/" + name + dir_str + ".png").scaled(size, size, Qt::KeepAspectRatio));
    setZValue(1.0);
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
