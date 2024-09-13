#ifndef ENTITY_H
#define ENTITY_H
#include <QGraphicsPixmapItem>
#include "shared.h"

#define DEFAULT_SPEED_CO 2
#define NO_OF_DIRECTIONS 4

struct DirVectors
{
    int x_co;
    int y_co;
    DirVectors(int x, int y) : x_co(x), y_co(y) {}
    DirVectors operator*(const int a) const
    {
        return DirVectors(this->x_co * a, this->y_co * a);
    }
    DirVectors operator*=(const int a)
    {
        return *this * a;
    }
};

class Entity : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Entity(const int size);
    virtual ~Entity() = default;
    void setNextDir(moveDirections next) {this->next_direction = next;};
    bool canChangeDir();
    bool setDir(moveDirections dir);
    virtual void move();
    const std::map<moveDirections, qreal> rotations =
        {
            {RIGHT, 0},
            {UP, -90},
            {LEFT, 180},
            {DOWN, 90}
        };

protected:
    double SPEED_CO = DEFAULT_SPEED_CO;
    QPixmap originalPixmap;
    virtual void rotateEntity(qreal angle) = 0;
    virtual bool canMove(DirVectors speed_vec) = 0;
    const int size;
    moveDirections direction;
    moveDirections next_direction;
    const DirVectors dir_vec[NO_OF_DIRECTIONS] =
    {
        DirVectors(0, -1),
        DirVectors(-1, 0),
        DirVectors(0, 1),
        DirVectors(1, 0)
    };

};

#endif // ENTITY_H
