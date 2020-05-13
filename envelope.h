#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <QColor>

class Envelope
{
public:
    Envelope();
    Envelope(int length);
    ~Envelope();

    int samples, fs;
    double *data;

    QColor *color;

    void setColor(int R, int G, int B, int A);
    void setFS(int fs);
};

#endif // ENVELOPE_H
