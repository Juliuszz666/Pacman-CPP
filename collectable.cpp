#include "collectable.h"

Collectable::Collectable(const collectable_type type, const int score, const int size, std::pair<int, int> pos):
    type(type),
    score(score),
    size(size)
{
    switch (type) {
    case FOOD:
        setPixmap(QPixmap(":/img/food.png").scaled(size, size, Qt::KeepAspectRatio));
        break;
    case POWER_UP:
        setPixmap(QPixmap(":/img/power_up.png").scaled(size, size, Qt::KeepAspectRatio));
        break;
    }
    auto [y, x] = pos;
    setPos(x * size, y * size);
}

QRectF Collectable::boundingRect() const
{
    return QRectF(0, 0, size, size);
}
