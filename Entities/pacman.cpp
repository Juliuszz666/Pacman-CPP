#include "pacman.h"
#include <QMessageBox>
#include "../MapElements/tile.h"

Pacman::Pacman(const int size, const std::pair<int, int> ini_pos) :
    Entity(size, ini_pos, ":/img/pacman.png"),
    number_of_lives(3)
{}

void Pacman::move()
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

void Pacman::reset()
{
    setPos(size, size);
    direction = NONE;
    setZValue(1.0);
    rotateEntity(rotations.at(RIGHT));
}

bool Pacman::canMove(DirVectors dir_vec)
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
        if (tile && (tile->getType() == WALL || tile->getType() == GHOST_GATE))
        {
            setPos(x, y);
            return false;
        }
    }
    setPos(x, y);
    return true;
}

void Pacman::rotateEntity(qreal angle)
{
    QTransform trans;
    trans.rotate(angle);
    setPixmap(originalPixmap);
    QPixmap rotated = pixmap().transformed(trans);
    setPixmap(rotated);
}
