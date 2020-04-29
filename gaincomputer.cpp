#include "gaincomputer.h"

GainComputer::GainComputer()
{
    threshold = 0;
    ratio = 1;
    kneeWidth = 0;
    makeupGain = 0;

    identity = new Envelope(97);

    for(int i = 0; i < identity->samples; i++)
    {
        identity->data[i] = i - 96;
    }
}

GainComputer::~GainComputer()
{
    delete[] identity;
    identity = nullptr;
}

void GainComputer::setThreshold(double T)
{
    threshold = T;
}
void GainComputer::setRatio(double R)
{
    ratio = R;
}
void GainComputer::setKneeWidth(double W)
{
    kneeWidth = W;
}
void GainComputer::setMakeupGain(double M)
{
    makeupGain = M;
}

double GainComputer::getThreshold()
{
    return threshold;
}
double GainComputer::getRatio()
{
    return ratio;
}
double GainComputer::getKneeWidth()
{
    return kneeWidth;
}
double GainComputer::getMakeupGain()
{
    return makeupGain;
}

double GainComputer::gainFunction(double x)
{
    double y;

    if((2*(x - threshold)) <= ((-1) * kneeWidth))
    {
        y = x;
    }
    else if((2 * qFabs(x - threshold)) < kneeWidth)
    {
        y = (x + (1/ratio-1)*qPow((x - threshold + kneeWidth/2), 2)/(2*kneeWidth));
    }
    else
    {
        y = (threshold + (x - threshold)/ratio);
    }

    return y;
}
