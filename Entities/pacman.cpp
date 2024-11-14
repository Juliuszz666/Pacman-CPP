#include "pacman.h"
#include <QMessageBox>
#include <QTimer>
#include "../shared.h"
#include "../MapElements/tile.h"

#define PACMAN_NORMAL_FILE_STR ":/img/pacman.png"
#define PACMAN_ANIMATED_FILE_STR ":/img/pacmananimated.png"

constexpr int ANIMATION_TIMER_MS = 150;


Pacman::Pacman(const int size, const std::pair<int, int> ini_pos) :
    Entity(size, ini_pos, ":/img/pacman.png"),
    number_of_lives(PACMAN_LIVES_DEFAULT),
    animation_timer(new QTimer(nullptr)),
    animation(false)
{
    animation_timer->start(ANIMATION_TIMER_MS);
    connect(animation_timer, &QTimer::timeout, this, &Pacman::animate);
}

Pacman::~Pacman()
{
    delete animation_timer;
}

void Pacman::setLives(int lives)
{
    this->number_of_lives = lives;
}

void Pacman::setAnimatedPixmap()
{
    SET_PIXMAP(PACMAN_ANIMATED_FILE_STR);
}

void Pacman::setNormalPixmap()
{
    SET_PIXMAP(PACMAN_NORMAL_FILE_STR);
}

void Pacman::move()
{
    if(direction == NONE) return;
    animation = false;
    auto [x_v, y_v] = dir_vec[direction] * SPEED_CO;
    int x = pos().x();
    int y = pos().y();
    if(canMove(dir_vec[direction]))
    {
        animation = true;
        setPos(x + x_v, y + y_v);
    }
}

void Pacman::animate()
{
    if(direction == NONE) return;
    if(!animation)
    {
        setNormalPixmap();
    }
    else
    {
        static bool state = true;
        if(state)
        {
            setAnimatedPixmap();
            state = false;
        }
        else
        {
            setNormalPixmap();
            state = true;
        }
    }
    originalPixmap = pixmap();
    rotateEntity(rotations.at(direction));
}

void Pacman::reset()
{
    setPos(size, size);
    direction = NONE;
    setZValue(1.0);
    setNormalPixmap();
    rotateEntity(rotations.at(RIGHT));
}

bool Pacman::canMove(DirVectors dir_vec)
{
    if(direction == NONE) return true;
    int x = pos().x();
    int y = pos().y();
    auto [x_test, y_test] = dir_vec * SPEED_CO;
    setPos(x + x_test, y + y_test);
    QList<QGraphicsItem*> collisions = collidingItems();

    for (auto item : collisions)
    {
        Tile* tile = dynamic_cast<Tile*>(item);
        if (tile && (tile->getType() == WALL || tile->getType() == GHOST_GATE))
        {
            setPos(x, y);
            return false;
        }
    }
    setPos(x, y);
    return true;
}

void Pacman::rotateEntity(qreal angle)
{
    QTransform trans;
    trans.rotate(angle);
    setPixmap(originalPixmap);
    QPixmap rotated = pixmap().transformed(trans);
    setPixmap(rotated);
}
