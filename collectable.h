#ifndef COLLECTABLE_H
#define COLLECTABLE_H

enum collectable_type
{
    FOOD,
    POWER_UP
};

class Collectable
{
protected:
    int score;
    Collectable();
    ~Collectable();
};

#endif // COLLECTABLE_H
