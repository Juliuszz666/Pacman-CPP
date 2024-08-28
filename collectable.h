#ifndef COLLECTABLE_H
#define COLLECTABLE_H

class Collectable
{
protected:
    int score;
    virtual void destroySelf() = 0;
    virtual ~Collectable() = 0;
};

#endif // COLLECTABLE_H
