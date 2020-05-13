#ifndef WAVE_H
#define WAVE_H

#include "QString"
#include "QFile"
#include "QMessageBox"
#include "QDataStream"
#include "qmath.h"

class wave
{
public:
    wave();
    wave(wave *src);
    wave(wave *src, int value);
    wave(int length);
    ~wave();

    enum endianess {little, big};

    void init();
    void displayError(QString message);
    int read_field(QFile &file, endianess order, short bytes);
    void write_2B(QFile &file, endianess order, short bytes);
    void write_4B(QFile &file, endianess order, int bytes);
    void load(QString path);
    void store(QString path);
    void mergeStereo(wave *stereo);
    void toAbs();

    int ChunkID;
    int ChunkSize;
    int Format;
    int Subchunk1ID;
    int Subchunk1Size;
    short AudioFormat;
    short NumChannels;
    int SampleRate;
    int ByteRate;
    short BlockAlign;
    short BitsPerSample;
    int Subchunk2ID;
    int Subchunk2Size;
    short* data;

    bool loaded;

};

#endif // WAVE_H
