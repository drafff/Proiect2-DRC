#include "leveldetector.h"

LevelDetector::LevelDetector()
{
    e = 2.7182818284590452;

    attack = 0;
    release = 0;
}

double LevelDetector::getAttack()
{
    return attack;
}
double LevelDetector::getRelease()
{
    return release;
}
void LevelDetector::setAttack(double attack)
{
    this->attack = attack;
}
void LevelDetector::setRelease(double release)
{
    this->release = release;
}

void LevelDetector::filter(Envelope *input, Envelope *output)
{
    double alphaA, alphaR;

    //alphaA = pow(e, (-1)/(attack/1000 * input->fs));
    //alphaR = pow(e, (-1)/(release/1000 * input->fs));

    alphaA = pow(0.1, 1/(attack/1000 * input->fs));
    alphaR = pow(0.1, 1/(release/1000 * input->fs));

    Envelope *y1 = new Envelope(input->samples);
    y1->setFS(input->fs);

    output->data[0] = 0;
    y1->data[0] = 0;

    for(int i = 1; i < input->samples; i++)
    {
        y1->data[i] = qMin(input->data[i], alphaR*y1->data[i-1] + (1 - alphaR)*input->data[i]);

        if(input->data[i] < output->data[i-1])
            output->data[i] = alphaA*output->data[i-1] + (1.0 - alphaA)*y1->data[i];
        else
            output->data[i] = alphaR*output->data[i-1] + (1.0 - alphaR)*input->data[i];
    }
}
