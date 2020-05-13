#include "envelope.h"

Envelope::Envelope()
{
    samples = 0;
    fs = 0;
    data = nullptr;

    color = new QColor(63, 171, 255, 255);
}

Envelope::Envelope(int length)
{
    samples = length;
    data = new double[samples];

    for(int i = 0; i < samples; i++)
    {
        data[i] = 0;
    }

    color = new QColor(63, 171, 255, 255);
}

Envelope::~Envelope()
{
    delete[] data;
    data = nullptr;
}

void Envelope::setColor(int R, int G, int B, int A)
{
    delete color;
    color = new QColor(R, G, B, A);
}

void Envelope::setFS(int fs)
{
    this->fs = fs;
}
