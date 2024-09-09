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

void Pacman::move()
{
    int x = pos().x();
    int y = pos().y();
    setPos(x + dir_vec[direction].x_co, y + dir_vec[direction].y_co);
    if(!canMove())
    {
        setPos(x, y);
    }
}
