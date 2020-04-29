#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <QColor>

class Envelope
{
public:
    Envelope();
    Envelope(int length);
    ~Envelope();

    int samples;
    double *data;

    QColor *color;

    void setColor(int R, int G, int B, int A);
};

#endif // ENVELOPE_H
