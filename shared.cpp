#include "shared.h"

QStack<int> Shared::pageIndexStack;

uint Shared::score;

const QMap<keyActions, int> Shared::defaultBindings =
{
        {MVUP, 'W'},
        {MVLEFT, 'A'},
        {MVDOWN, 'S'},
        {MVRIGHT, 'D'},
        {SETTINGS, KEY_ESCAPE},
        {PAUSE, 'P'}
};

QMap<keyActions, int> Shared::keyBindings = Shared::defaultBindings;
