#include "tile.h"
#include <QBrush>
#include <QPen>

#define FLOOR_COLOR QColor(Qt::blue))
#define WALL_COLOR QColor(Qt::black))
#define GHOST_GATE_COLOR QColor(Qt::green))

Tile::Tile(tileType type, int size, std::pair<int, int> pos) :
    type(type),
    size(size)
{
    setRect(QRectF(0, 0, size, size));
    switch (type) {
    case FLOOR:
        setBrush(QBrush(FLOOR_COLOR);
        break;
    case WALL:
        setBrush(QBrush(WALL_COLOR);
        break;
    case GHOST_GATE:
        setBrush(QBrush(GHOST_GATE_COLOR);
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
