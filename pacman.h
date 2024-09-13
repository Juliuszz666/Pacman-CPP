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
    moveDirections getDir() {return direction;}

private:
    virtual bool canMove(DirVectors dir_vec) override;
    virtual void rotateEntity(qreal angle) override;
};

#endif // PACMAN_H
