#ifndef ENTITY_H
#define ENTITY_H
#include <QGraphicsPixmapItem>
#include "shared.h"

#define NO_OF_DIRECTIONS 4

struct DirVectors
{
    DirVectors(int x, int y) : x_co(x), y_co(y) {}
    int x_co;
    int y_co;
    DirVectors operator*(const double a) const
    {
        return DirVectors((int)(this->x_co * a), (int)(this->y_co * a));
    }
    DirVectors operator*=(const double a)
    {
        return *this * a;
    }
};

class Entity : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Entity(const int size) : size(size) {};
    virtual ~Entity() = default;

protected:
    virtual void move() = 0;
    virtual bool canMove() const;
    const int size;
    moveDirections direction;
    const DirVectors dir_vec[NO_OF_DIRECTIONS] =
    {
        DirVectors(0, -1),
        DirVectors(-1, 0),
        DirVectors(0, 1),
        DirVectors(1, 0)
    };

};

#endif // ENTITY_H
