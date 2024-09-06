#ifndef SHARED_H
#define SHARED_H

#include <QtMultimedia>
#include <QPushButton>

#define KEY_ESCAPE 16777216
#define WELCOME_PAGE 0
#define GAME_PAGE 1
#define SETTINGS_PAGE 2
#define LEVELS_FILE ":/levels.json"
#define MAP_BLANK 0
#define MAP_WALL 1
#define MAP_POWER_UP 2
#define MAP_FOOD 3


enum keyActions
{
    MVUP,
    MVLEFT,
    MVDOWN,
    MVRIGHT,
    PAUSE,
    SETTINGS
};

class Shared
{
public:

    friend class GamePage;
    friend class MainWindow;
    friend class SettingsPage;
private:
    static QStack<int> pageIndexStack;
    static std::unique_ptr<QMediaPlayer> music_player;
    static std::unique_ptr<QAudioOutput> audio_output;
    static QMap<keyActions, int> keyBindings;
    static const QMap<keyActions, int> defaultBindings;
public:

    Shared() = default;
    ~Shared() = default;
};

#endif // SHARED_H
