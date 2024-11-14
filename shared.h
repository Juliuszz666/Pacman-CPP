#ifndef SHARED_H
#define SHARED_H

#include <QPushButton>
#include <QStack>

#define KEY_ESCAPE 16777216
#define WELCOME_PAGE 0
#define GAME_PAGE 1
#define SETTINGS_PAGE 2
#define GAME_OVER_PAGE 3
#define LEVELS_FILE ":/levels.json"
#define MAP_BLANK 0
#define MAP_WALL 1
#define MAP_POWER_UP 2
#define MAP_FOOD 3
#define __GHOST_GATE 4
#define MAP_WIDTH 30
#define MAP_HEIGHT 20

constexpr double TIMER_COLAPSE_TIME = (1000/30.0);      // 30 FPS
constexpr int POWER_UP_TIME = 10'000;                   //ms

#define SET_PIXMAP(x) setPixmap(QPixmap(x).scaled(size, size, Qt::KeepAspectRatio))

constexpr int PACMAN_LIVES_DEFAULT = 3;

enum keyActions
{
    MVUP,
    MVLEFT,
    MVDOWN,
    MVRIGHT,
    PAUSE,
    SETTINGS
};

enum moveDirections
{
    UP,
    LEFT,
    DOWN,
    RIGHT,
    NONE
};

enum GhostState
{
    EDIBLE,
    INEDIBLE
};

class Shared
{
public:
    static uint score;
    static QStack<int> pageIndexStack;
    static QMap<keyActions, int> keyBindings;
    static const QMap<keyActions, int> defaultBindings;
public:
    Shared() = default;
    ~Shared() = default;
};

#endif // SHARED_H
