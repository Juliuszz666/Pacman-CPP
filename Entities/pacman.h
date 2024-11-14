#ifndef PACMAN_H
#define PACMAN_H
#include "entity.h"
#include <QKeyEvent>

class Pacman : public Entity
{
    Q_OBJECT
public:
    Pacman(const int size, const std::pair<int, int> ini_pos);
    ~Pacman() override;
    bool loseLife();
    uint getLife();
    void setLives(uint lives);
    void move() override;
    void reset();
private:
    void setAnimatedPixmap();
    void setNormalPixmap();
    QTimer *animation_timer;
    uint number_of_lives;
    void animate();
    bool animation;
    virtual bool canMove(DirVectors dir_vec) override;
    virtual void rotateEntity(qreal angle) override;
};

#endif // PACMAN_H
