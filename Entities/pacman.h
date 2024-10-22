#ifndef PACMAN_H
#define PACMAN_H
#include "../shared.h"
#include "entity.h"
#include <QKeyEvent>

class Pacman : public Entity
{
    Q_OBJECT
public:
    Pacman(const int size, const std::pair<int, int> ini_pos);
    ~Pacman() override = default;
    bool loseLife() {return --number_of_lives;}
    uint getLife() {return number_of_lives;}
    void move() override;
    void reset();
private:
    uint number_of_lives;
    virtual bool canMove(DirVectors dir_vec) override;
    virtual void rotateEntity(qreal angle) override;
};

#endif // PACMAN_H
