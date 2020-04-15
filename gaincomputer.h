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

    double getThreshold();
    double getRatio();
    double getKneeWidth();

    Envelope *identity;

    double gainFunction(double x);

private:

    double threshold, ratio, kneeWidth;
};

#endif // GAINCOMPUTER_H
