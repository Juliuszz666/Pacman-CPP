#include "pinky.h"
#include <QTimer>

#define RESET_TIME 2000

void Pinky::returnToSpawn()
{
    QTimer* reset_timer = new QTimer(nullptr);
    connect(reset_timer, &QTimer::timeout, reset_timer, &QTimer::stop);
    connect(reset_timer, &QTimer::destroyed, reset_timer, &QTimer::deleteLater);
    connect(reset_timer, &QTimer::timeout, this, [&]()
    {
        setPos(15 * size, 5 * size);
        state = INEDIBLE;
    });
    reset_timer->start(RESET_TIME);
}

void Pinky::move()
{
    qDebug() << "zrób to";
}
