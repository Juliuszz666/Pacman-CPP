#ifndef GHOST_H
#define GHOST_H
#include "entity.h"

class Ghost : public Entity
{
protected:
    GhostState state;
    QPoint pacman_pos;
public:
    GhostState getState() {return state;}
    void setState(GhostState newstate) {this->state = newstate;}
    void getPacmanPos(QPoint pos) {this->pacman_pos = pos;}
    virtual void returnToSpawn() = 0;
    virtual void move() override = 0;
    virtual void rotateEntity(qreal angle) override;
    virtual bool canMove(DirVectors speed_vec) override;
    Ghost(const int size, const std::pair<int, int> ini_pos, const QString &img_filename) :
        Entity(size, ini_pos, img_filename),
        state(INEDIBLE),
        pacman_pos({}) {}
};

#endif // GHOST_H
