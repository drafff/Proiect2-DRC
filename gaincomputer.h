#ifndef GAINCOMPUTER_H
#define GAINCOMPUTER_H

#include <envelope.h>
#include <QtMath>

class GainComputer
{
public:
    GainComputer();
    ~GainComputer();

    void setThreshold(double T);
    void setRatio(double R);
    void setKneeWidth(double W);
    void setMakeupGain(double M);

    double getThreshold();
    double getRatio();
    double getKneeWidth();
    double getMakeupGain();

    Envelope *identity;

    double gainFunction(double x);

private:

    double threshold, ratio, kneeWidth, makeupGain;
};

#endif // GAINCOMPUTER_H
