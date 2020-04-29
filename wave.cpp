#include "wave.h"

wave::wave()
{
    ChunkID = 0;
    ChunkSize = 0;
    Format = 0;
    Subchunk1ID = 0;
    Subchunk1Size = 0;
    AudioFormat = 0;
    NumChannels = 0;
    SampleRate = 0;
    ByteRate = 0;
    BlockAlign = 0;
    BitsPerSample = 0;
    Subchunk2ID = 0;
    Subchunk2Size = 0;
    data = nullptr;

    loaded = false;
}

wave::wave(wave *src)
{
    wave::init();

    this->ChunkID = src->ChunkID;
    this->ChunkSize = src->ChunkSize;
    this->Format = src->Format;
    this->Subchunk1ID = src->Subchunk1ID;
    this->Subchunk1Size = src->Subchunk1Size;
    this->AudioFormat = src->AudioFormat;
    this->NumChannels = src->NumChannels;
    this->SampleRate = src->SampleRate;
    this->ByteRate = src->ByteRate;
    this->BlockAlign = src->BlockAlign;
    this->BitsPerSample = src->BitsPerSample;
    this->Subchunk2ID = src->Subchunk2ID;
    this->Subchunk2Size = src->Subchunk2Size;
    this->data = new short(this->Subchunk2Size / 2);

    if(this->data == nullptr)
    {
        displayError(*new QString("Memory allocation error."));
        wave::init();
        return;
    }

    for(int i = 0; i < this->Subchunk2Size / 2; i++)
    {
        this->data[i] = src->data[i];
    }

    this->loaded = src->loaded;
}

wave::wave(int length)
{
    Subchunk2Size = length;

    ChunkID = (int)"RIFF";
    ChunkSize = 36 + Subchunk2Size;
    Format = (int)"WAVE";
    Subchunk1ID = (int)"fmt ";
    Subchunk1Size = 16;
    AudioFormat = 1;
    NumChannels = 1;
    SampleRate = 44100;
    ByteRate = SampleRate * 2;
    BlockAlign = 2;
    BitsPerSample = 16;
    Subchunk2ID = (int)"data";
    data = new short[Subchunk2Size / 2];

    if(data == nullptr)
    {
        displayError(*new QString("Memory allocation error."));
        wave::init();
        return;
    }

    for (int i = 0; i < Subchunk2Size / 2; i++)
        data[i] = 0;

    loaded = true;
}

wave::~wave()
{
    delete[] data;
    data = nullptr;
}

void wave::init()
{
    ChunkID = 0;
    ChunkSize = 0;
    Format = 0;
    Subchunk1ID = 0;
    Subchunk1Size = 0;
    AudioFormat = 0;
    NumChannels = 0;
    SampleRate = 0;
    ByteRate = 0;
    BlockAlign = 0;
    BitsPerSample = 0;
    Subchunk2ID = 0;
    Subchunk2Size = 0;

    delete[] data;

    data = nullptr;

    loaded = false;
}

void wave::displayError(QString message)
{
    QMessageBox messageBox;
    messageBox.critical(0,"Error", message);
    messageBox.setFixedSize(500,200);

    return;
}

int wave::read_field(QFile &file, endianess order, short bytes)
{
    QDataStream in(&file);
    char buffer2[2] = {0, 0}, buffer4[4] = {0, 0, 0, 0};

    unsigned char temp;

    int ret = 0;

    if(bytes == 2)
    {
        in.readRawData(buffer2, 2);

        if(order == little)
        {
            for(int i = 0; i < 2; i++)
            {
                temp = buffer2[i];
                ret = ret + (((int)temp) << (8*i));
            }
        }
        else
        {
            for(int i = 0; i < 2; i++)
            {
                temp = buffer2[i];
                ret = (int)temp + (ret << 8);
            }
        }

        return (short)ret;
    }
    else
    {
        in.readRawData(buffer4, 4);

        if(order == little)
        {
            for(int i = 0; i < 4; i++)
            {
                temp = buffer4[i];
                ret = ret + (((int)temp) << (8*i));
            }
        }
        else
        {
            for(int i = 0; i < 4; i++)
            {
                temp = buffer4[i];
                ret = (int)temp + (ret << 8);
            }
        }

        return ret;
    }
}

void wave::load(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        displayError(*new QString("File could not be opened."));
        return;
    }

    if((ChunkID = wave::read_field(file, big, 4)) != 0x52494646) {displayError(*new QString("File could not be loaded. File is not RIFF.")); wave::init(); file.close(); return;}
    ChunkSize = wave::read_field(file, little, 4);
    if((Format = wave::read_field(file, big, 4)) != 0x57415645) {displayError(*new QString("File could not be loaded. File is not WAVE.")); wave::init(); file.close(); return;}
    if((Subchunk1ID = wave::read_field(file, big, 4)) != 0x666D7420) {displayError(*new QString("File could not be loaded. Subchunk1 is not fmt.")); wave::init(); file.close(); return;}
    if((Subchunk1Size = wave::read_field(file, little, 4)) != 0x00000010) {displayError(*new QString("File could not be loaded. Subchunk1 doesn't match fmt format.")); wave::init(); file.close(); return;}
    if((AudioFormat = wave::read_field(file, little, 2)) != 0x0001) {displayError(*new QString("File could not be loaded. Quantization is not linear.")); wave::init(); file.close(); return;}
    if((NumChannels = wave::read_field(file, little, 2)) > 0x0002) {displayError(*new QString("File could not be loaded. Audio has more than 2 channels.")); wave::init(); file.close(); return;}
    SampleRate = wave::read_field(file, little, 4);
    ByteRate = wave::read_field(file, little, 4);
    BlockAlign = wave::read_field(file, little, 2);
    if((BitsPerSample = wave::read_field(file, little, 2)) != 0x0010) {displayError(*new QString("File could not be loaded. Audio is not 16-bit.")); wave::init(); file.close(); return;}
    if((Subchunk2ID = wave::read_field(file, big, 4)) != 0x64617461) {displayError(*new QString("File could not be loaded. Subchunk2 does not contain sample data.")); wave::init(); file.close(); return;}
    Subchunk2Size = wave::read_field(file, little, 4);

    data = new short[Subchunk2Size / 2];

    if(data == nullptr)
    {
        displayError(*new QString("Memory allocation error."));
        wave::init();
        file.close();
        return;
    }

    for (int i = 0; i < Subchunk2Size / 2; i++)
        {    
            data[i] = 0 + read_field(file, little, 2);
        }

    loaded = true;
    file.close();
}

void wave::mergeStereo(wave *stereo)
{
    this->ChunkID = stereo->ChunkID;
    this->ChunkSize = stereo->ChunkSize;
    this->Format = stereo->Format;
    this->Subchunk1ID = stereo->Subchunk1ID;
    this->Subchunk1Size = stereo->Subchunk1Size;
    this->AudioFormat = stereo->AudioFormat;
    this->NumChannels = 1;
    this->SampleRate = stereo->SampleRate;
    this->ByteRate = stereo->ByteRate;
    this->BlockAlign = stereo->BlockAlign;
    this->BitsPerSample = stereo->BitsPerSample;
    this->Subchunk2ID = stereo->Subchunk2ID;
    this->Subchunk2Size = stereo->Subchunk2Size/2;

    data = new short[this->Subchunk2Size / 2];

    if(data == nullptr)
    {
        displayError(*new QString("Memory allocation error."));
        wave::init();
        return;
    }

    for (int i = 0; i < this->NumChannels * this->Subchunk2Size / 2; i++)
        {
            this->data[i] = (qAbs(stereo->data[2*i])+qAbs(stereo->data[2*i+1]))/2;
        }

    this->loaded = true;
}

void wave::toAbs()
{
    if(this->loaded != true)
    {
        displayError(*new QString("Call of toAbs() member function in uninitialized wave."));
        return;
    }

    for(int i = 0; i < Subchunk2Size / 2; i++)
    {
        data[i] = qAbs(data[i]);
    }

    return;
}











