#include "pacman.h"
#include <QMessageBox>

Pacman::Pacman(const int size, const std::pair<int, int> ini_pos) :
    Entity(size)
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
    setPixmap(QPixmap(":/img/pacman.png").scaled(size, size, Qt::KeepAspectRatio));
    auto [y, x] = ini_pos;
    setPos(x * size, y * size);
    this->originalPixmap = pixmap();
}

QRectF Pacman::boundingRect() const
{
    return QRectF(0, 0, size, size);
}



QPainterPath Pacman::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

bool Pacman::move()
{
    if(direction == NONE) return true;
    const double SPEED_CO = 1.5;
    DirVectors speed_vec = dir_vec[direction] * SPEED_CO;
    auto [x_v, y_v] = speed_vec;
    int x = pos().x();
    int y = pos().y();
    setPos(x + x_v, y + y_v);
    if(!canMove())
    {
        setPos(x, y);
        return false;
    }
    return true;
}

void Pacman::setDir(moveDirections dir)
{
    moveDirections current = direction;
    direction = dir;

    if(!move())
    {
        direction = current;
    }
}

void Pacman::rotatePlayer(qreal angle)
{
    QTransform trans;
    trans.rotate(angle);
    setPixmap(originalPixmap);
    QPixmap rotated = pixmap().transformed(trans);
    setPixmap(rotated);
}
