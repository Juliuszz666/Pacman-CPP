#ifndef SHARED_H
#define SHARED_H

#include <QPushButton>
#include <QStack>

#define LEVELS_FILE ":/levels.json"
#define SET_PIXMAP(x) setPixmap(QPixmap(x).scaled(size, size, Qt::KeepAspectRatio))

constexpr int KEY_ESCAPE = 16777216;
constexpr int WELCOME_PAGE = 0;
constexpr int GAME_PAGE = 1;
constexpr int SETTINGS_PAGE = 2;
constexpr int GAME_OVER_PAGE = 3;

constexpr int MAP_BLANK = 0;
constexpr int MAP_WALL = 1;
constexpr int MAP_POWER_UP = 2;
constexpr int MAP_FOOD = 3;
constexpr int __GHOST_GATE = 4;

constexpr int MAP_WIDTH = 30;
constexpr int MAP_HEIGHT = 20;
constexpr double TIMER_COLAPSE_TIME = (1000/30.0);      // 30 FPS
constexpr int POWER_UP_TIME = 10'000;                   //ms

constexpr int PACMAN_LIVES_DEFAULT = 3;

constexpr int CELL_SIZE = 20;

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
