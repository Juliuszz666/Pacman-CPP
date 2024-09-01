#ifndef COLLECTABLE_H
#define COLLECTABLE_H

class Collectable
{
protected:
    int score;
    Collectable() = default;
    virtual ~Collectable() = 0;
};

#endif // COLLECTABLE_H
