#ifndef GHOST_H
#define GHOST_H
#include "entity.h"
#include <QTimer>

constexpr int RESET_TIME_MS = 3500;

moveDirections leftTurn(moveDirections curr);
moveDirections rightTurn(moveDirections curr);
moveDirections backTurn(moveDirections curr);

class Ghost : public Entity
{
protected:
    GhostState state;
    QPoint pacman_pos;
    bool eaten;
    std::pair<int,int> spawn_pos;
    std::pair<int,int> gate_pos;
    QString name;
public:
    QTimer* reset_timer;
    GhostState getState() {return state;}
    void setState(GhostState newstate);
    void getPacmanPos(QPoint pos) {this->pacman_pos = pos;}
    void returnToSpawn();
    virtual void move() override = 0;
    virtual void rotateEntity(qreal angle) override;
    virtual bool canMove(DirVectors speed_vec) override;
    Ghost(const int size, const std::pair<int, int> ini_pos, const QString &img_filename, const std::pair<int,int> g_pos, const QString& name);
    ~Ghost();
};

#endif // GHOST_H
