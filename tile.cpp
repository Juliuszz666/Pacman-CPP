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
        setBrush(QBrush(QColor(Qt::white)));
        setPen(QPen(Qt::NoPen));
        break;
    case WALL:
        setBrush(QBrush(QColor(Qt::black)));
        setPen(QPen(Qt::NoPen));
        break;
    }
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
