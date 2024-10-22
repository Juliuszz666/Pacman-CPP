#include "entity.h"

Entity::Entity(const int size, const std::pair<int, int> ini_pos, const QString &img_filename) :
    size(size),
    direction(NONE),
    next_direction(NONE),
    originalPixmap(this->pixmap())
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
    setPixmap(QPixmap(img_filename).scaled(size, size, Qt::KeepAspectRatio));
    auto [y, x] = ini_pos;
    setPos(x * size, y * size);
    originalPixmap = this->pixmap();
}

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

QRectF Entity::boundingRect() const
{
    return QRectF(0, 0, size, size);
}

QPainterPath Entity::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}
