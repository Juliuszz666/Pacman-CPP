#include "tile.h"
#include <QBrush>
#include <QPen>

Tile::Tile(tileType type, int size, std::pair<int, int> pos) :
    type(type),
    size(size)
{
    setRect(QRectF(0, 0, size, size));
    switch (type) {
    case FLOOR:
        setBrush(QBrush(QColor(Qt::blue)));
        break;
    case WALL:
        setBrush(QBrush(QColor(Qt::black)));
        break;
    case GHOST_GATE:
        setBrush(QBrush(QColor(Qt::green)));
        break;
    }
    setPen(QPen(Qt::NoPen));
    auto [y, x] = pos;
    setPos(x * size, y * size);
}

QRectF Tile::boundingRect() const
{
    return QRectF(0, 0, size, size);
}

QPainterPath Tile::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

tileType Tile::getType() const
{
    return type;
}
