#include "ghost.h"
#include "../MapElements/tile.h"
#include <QTimer>
#include <QPixmap>

#define GHOST_EDBILE_FILE_STR ":/img/ghost_edible.png"

Ghost::Ghost(const int size,
             const std::pair<int, int> ini_pos,
             const QString &img_filename,
             const std::pair<int,int> g_pos,
             const QString& name) :
    Entity(size, ini_pos, img_filename),
    state(INEDIBLE),
    spawn_pos(ini_pos),
    reset_timer(new QTimer(nullptr)),
    pacman_pos({}),
    gate_pos(g_pos),
    name(name),
    eaten(false)
{}

Ghost::~Ghost()
{
    delete reset_timer;
}

QString getDirStr(moveDirections direction)
{
    switch (direction) {
    case LEFT:
        return "left";
        break;
    case UP:
        return "up";
        break;
    case RIGHT:
        return "right";
        break;
    case DOWN:
        return "down";
        break;
    default:
        return "";
    }
}

void Ghost::setState(GhostState newState)
{
    this->state = newState;
    switch (newState) {
    case EDIBLE:
        SET_PIXMAP(GHOST_EDBILE_FILE_STR);
        break;
    case INEDIBLE:
        QString dir_str = getDirStr(direction);
        SET_PIXMAP(":/img/" + name + dir_str + ".png");
        break;
    }
    setZValue(1.0);
    originalPixmap = pixmap();
}

bool Ghost::canMove(DirVectors dir_vec)
{
    if(eaten) return false;
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
    if(state == EDIBLE) return;
    QString dir_str = getDirStr(direction);
    SET_PIXMAP(":/img/" + name + dir_str + ".png");
    setZValue(1.0);
}

void Ghost::returnToSpawn()
{
    reset_timer->start(RESET_TIME_MS);
    setPos(spawn_pos.second * size, spawn_pos.first * size);
    setDir(UP);
    SET_PIXMAP(":/img/" + name + "up.png");
    originalPixmap = pixmap();
    setZValue(1.0);
    eaten = true;
    connect(reset_timer, &QTimer::timeout, reset_timer, &QTimer::stop);
    connect(reset_timer, &QTimer::destroyed, reset_timer, &QTimer::deleteLater);
    connect(reset_timer, &QTimer::timeout, this, [&]()
    {
        setState(INEDIBLE);
        eaten = false;
    });
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
