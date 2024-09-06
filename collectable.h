#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include <QGraphicsItem>

enum collectable_type
{
    FOOD,
    POWER_UP
};

class Collectable : public QGraphicsPixmapItem
{
public:
    Collectable(const collectable_type type, const int score,
                const int size, std::pair<int, int> pos);
    ~Collectable() = default;
    QRectF boundingRect() const override;
    //QPainterPath shape() const override;

private:
    collectable_type type;
    const int score;
    const int size;
};

#endif // COLLECTABLE_H
