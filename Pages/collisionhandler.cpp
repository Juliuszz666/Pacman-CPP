#include "collisionhandler.h"

constexpr int GHOST_EATING_POINTS = 200;


void CollisionHandler::collectablesCollisions(GamePage *game_instance, const QList<QGraphicsItem*> &collisions)
{
    for (const auto &item : collisions)
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
        }
    }
    if(game_instance->collectables.empty())
    {
        if(game_instance->current_level == game_instance->max_level)
        {
            game_instance->gameOver();
        }
        else
        {
            game_instance->newLevel();
        }
    }
}

void CollisionHandler::ghostCollisions(GamePage *game_instance, const QList<QGraphicsItem*> &collisions)
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
                    game_instance->newLifeRestart();
                }
                else
                {
                    game_instance->gameOver();
                    emit game_instance->gameOverSignal();
                    return;
                }
                break;
            }
        }
    }
}

void CollisionHandler::handlePacmanCollisions(GamePage *game_instance)
{
    QList<QGraphicsItem*> collisions = game_instance->pacman->collidingItems();
    collectablesCollisions(game_instance, collisions);
    ghostCollisions(game_instance, collisions);
}
