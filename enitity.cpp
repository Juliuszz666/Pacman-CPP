#include "entity.h"
#include "tile.h"

bool Entity::canMove() const
{
    QList<QGraphicsItem*> collisions = collidingItems();
    for (auto item : collisions)
    {
        Tile* tile = dynamic_cast<Tile*>(item);
        if (tile && tile->getType() == WALL)
        {
            return false;
        }
    }
    return true;
}
