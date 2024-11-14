#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "gamepage.h"

class CollisionHandler
{
private:
    void ghostCollisions(GamePage *page, const QList<QGraphicsItem*> &collisions);
    void collectablesCollisions(GamePage *page, const QList<QGraphicsItem*> &collisions);
public:
    CollisionHandler() = default;
    ~CollisionHandler() = default;
public slots:
    void handlePacmanCollisions(GamePage *page);
};

#endif // COLLISIONHANDLER_H
