#include "shared.h"

QStack<int> Shared::pageIndexStack;

const QMap<keyActions, int> Shared::defaultBindings =
{
        {MVUP, 'W'},
        {MVLEFT, 'A'},
        {MVDOWN, 'S'},
        {MVRIGHT, 'D'},
        {SETTINGS, KEY_ESCAPE},
        {PAUSE, 'P'}
};

std::unique_ptr<QAudioOutput> Shared::audio_output = std::make_unique<QAudioOutput>();
std::unique_ptr<QMediaPlayer> Shared::music_player = std::make_unique<QMediaPlayer>();

QMap<keyActions, int> Shared::keyBindings = Shared::defaultBindings;
