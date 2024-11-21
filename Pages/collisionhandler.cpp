#include "collisionhandler.h"

constexpr int GHOST_EATING_POINTS = 200;

constexpr int NEW_LEVEL_FLAG = 0x1;
constexpr int NEW_LIFE_RESTART_FLAG = 0x2;
constexpr int GAME_OVER_FLAG = 0x4;


void CollisionHandler::collectablesCollisions(GamePage *game_instance, QList<QGraphicsItem*> &collisions, int & flag)
{
    for (auto &item : collisions)
    {
        Collectable* collectable = dynamic_cast<Collectable*>(item);
        if (collectable)
        {
            if(collectable->getType() == POWER_UP)
            {
                game_instance->powerUpMode();
            }
            auto it = std::find(game_instance->collectables.begin(), game_instance->collectables.end(), collectable);
            Shared::score += collectable->getScore();
            game_instance->scene->removeItem(item);
            game_instance->collectables.erase(it);
            delete item;
            item = nullptr;
        }
    }
    if(game_instance->collectables.empty())
    {
        if(game_instance->current_level == game_instance->max_level)
        {
            flag |= GAME_OVER_FLAG;
        }
        else
        {
            flag |= NEW_LEVEL_FLAG;
        }
    }
}

void CollisionHandler::ghostCollisions(GamePage *game_instance, const QList<QGraphicsItem*> &collisions, int &flag)
{
    for (const auto &item : collisions)
    {
        Ghost* ghost = dynamic_cast<Ghost*>(item);
        if(ghost)
        {
            switch (ghost->getState()) {
            case EDIBLE:
                ghost->returnToSpawn();
                Shared::score += GHOST_EATING_POINTS;
                break;
            case INEDIBLE:
                if(game_instance->pacman->loseLife())
                {
                    flag |= NEW_LIFE_RESTART_FLAG;
                }
                else
                {
                    flag |= GAME_OVER_FLAG;
                }
                break;
            }
        }
    }
}

void CollisionHandler::handlePacmanCollisions(GamePage *game_instance)
{
    int flag = 0x0;
    QList<QGraphicsItem*> collisions = game_instance->pacman->collidingItems();
    ghostCollisions(game_instance, collisions, flag);
    collectablesCollisions(game_instance, collisions, flag);
    if(flag & GAME_OVER_FLAG) game_instance->gameOver();
    if(flag & NEW_LIFE_RESTART_FLAG) game_instance->newLifeRestart();
    if(flag & NEW_LEVEL_FLAG) game_instance->newLevel();
}
