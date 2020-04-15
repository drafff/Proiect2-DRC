#include "envelope.h"

Envelope::Envelope()
{
    samples = 0;
    data = nullptr;
}

Envelope::Envelope(int length)
{
    samples = length;
    data = new short[samples];

    for(int i = 0; i < samples; i++)
    {
        data[i] = 0;
    }
}

Envelope::~Envelope()
{
    delete[] data;
    data = nullptr;
}
