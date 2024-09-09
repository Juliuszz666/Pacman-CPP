#ifndef PACMAN_H
#define PACMAN_H
#include "shared.h"
#include "entity.h"
#include <QKeyEvent>

class Pacman : public Entity
{
    Q_OBJECT
public:
    Pacman(const int size, const std::pair<int, int> ini_pos);
    ~Pacman() override = default;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setDir(moveDirections dir) {this->direction = dir;}
public slots:
    void move() override;
};

#endif // PACMAN_H
