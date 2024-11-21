#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "gamepage.h"

class CollisionHandler
{
private:
    void ghostCollisions(GamePage *page, const QList<QGraphicsItem*> &collisions, int &flag);
    void collectablesCollisions(GamePage *page, const QList<QGraphicsItem*> &collisions, int &flag);
public:
    CollisionHandler() = default;
    ~CollisionHandler() = default;
public slots:
    void handlePacmanCollisions(GamePage *page);
};

#endif // COLLISIONHANDLER_H
