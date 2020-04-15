#ifndef ENVELOPE_H
#define ENVELOPE_H


class Envelope
{
public:
    Envelope();
    Envelope(int length);
    ~Envelope();

    int samples;
    short *data;
};

#endif // ENVELOPE_H
