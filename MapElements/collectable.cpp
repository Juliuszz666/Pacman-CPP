#include "collectable.h"
#include "../shared.h"

#define FOOD_FILE_STR ":/img/food.png"
#define POWER_UP_FILE_STR ":/img/powerup.png"

Collectable::Collectable(const collectable_type type, const int score, const int size, std::pair<int, int> pos):
    type(type),
    score(score),
    size(size)
{
    switch (type) {
    case FOOD:
        SET_PIXMAP(FOOD_FILE_STR);
        break;
    case POWER_UP:
        SET_PIXMAP(POWER_UP_FILE_STR);
        break;
    }
    auto [y, x] = pos;
    setPos(x * size, y * size);
}

QRectF Collectable::boundingRect() const
{
    return QRectF(0, 0, size, size);
}

QPainterPath Collectable::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}
