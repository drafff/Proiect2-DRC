#ifndef LEVELDETECTOR_H
#define LEVELDETECTOR_H

#include <envelope.h>
#include <QtMath>

class LevelDetector
{
    double attack, release, e;

public:
    LevelDetector();

    void setAttack(double attack);
    void setRelease(double release);
    double getAttack();
    double getRelease();

    void filter(Envelope *input, Envelope *output);

};

#endif // LEVELDETECTOR_H
