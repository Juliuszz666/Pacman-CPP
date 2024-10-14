#ifndef TILE_H
#define TILE_H

#include <QGraphicsRectItem>

enum tileType
{
    FLOOR,
    WALL,
    GHOST_GATE
};

class Tile : public QGraphicsRectItem
{
private:
    const tileType type;
    const int size;

public:
    Tile(tileType type, int size, std::pair<int, int> pos);
    ~Tile() = default;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    tileType getType() const;
};

#endif // TILE_H
