#include "pacman.h"
#include <QMessageBox>
#include "tile.h"

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
        if (tile && tile->getType() == WALL)
        {
            setPos(x, y);
            return false;
        }
    }
    setPos(x, y);
    return true;
}

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

void Pacman::rotateEntity(qreal angle)
{
    QTransform trans;
    trans.rotate(angle);
    setPixmap(originalPixmap);
    QPixmap rotated = pixmap().transformed(trans);
    setPixmap(rotated);
}
