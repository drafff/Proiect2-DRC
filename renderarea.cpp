#include "renderarea.h"

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    pen = QPen(QColor(127, 127, 255, 255));

    sound = nullptr;
    env1 = nullptr;
    gainComputer = nullptr;
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 800);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(400, 400);
}

int RenderArea::getMax(wave *audio, unsigned int start, unsigned int end, int increment)
{
    int max = 0;

    for(unsigned int i = (start + 1)*increment; i < end*increment; i = i + increment)
    {
        if(audio->data[i] > max)
        {
            max = audio->data[i];
        }
    }

    return max;
}

int RenderArea::getMax(Envelope *env, unsigned int start, unsigned int end)
{
    int max = 0;

    for(unsigned int i = start + 1; i < end; i++)
    {
        if(env->data[i] > max)
        {
            max = env->data[i];
        }
    }

    return max;
}

int RenderArea::getMin(wave *audio, unsigned int start, unsigned int end, int increment)
{
    int min = 0;

    for(unsigned int i = (start + 1)*increment; i < end*increment; i = i + increment)
    {
        if(audio->data[i] < min)
        {
            min = audio->data[i];
        }
    }

    return min;
}

void RenderArea::plotWave(wave *wav)
{
    QPainter painter(this);
    QPen wavePen = QPen(QColor(63, 171, 255, 255));
    painter.setPen(wavePen);

    if(wav->NumChannels == 2)
    {
        unsigned int sampleCount = wav->Subchunk2Size / 4;

        double scale = (double)(sampleCount - 1)/(double)width();

        double x1, x2, y1, y2;

        if(scale > 2)
        {
            painter.setRenderHint(QPainter::Antialiasing, false);

            for(int i = 0; i < width(); i++)
            {
                x1 = i;
                y1 = height()/2 - (double)getMin(wav, i*(double)scale, (i+1)*scale, 2)/(double)65355*height();
                x2 = i;
                y2 = height()/2 - (double)getMax(wav, i*(double)scale, (i+1)*scale, 2)/(double)65355*height();

                painter.drawLine(x1, y1, x2, y2);

                y1 = height()/2 - (double)getMin(wav, 1+i*(double)scale, 1+(i+1)*scale, 2)/(double)65355*height();
                y2 = height()/2 - (double)getMax(wav, 1+i*(double)scale, 1+(i+1)*scale, 2)/(double)65355*height();

                painter.drawLine(x1, y1, x2, y2);
            }
        }
        else
        {
            painter.setRenderHint(QPainter::Antialiasing, true);

            for(unsigned int i = 0; i < sampleCount - 1; i++)
            {
                x1 = (double)i/scale;
                y1 = height()/2 - (double)wav->data[2*i]/(double)65355*height();
                x2 = (double)(i+1)/scale;
                y2 = height()/2 - (double)wav->data[2*(i+1)]/(double)65355*height();

                painter.drawLine(x1, y1, x2, y2);

                y1 = height()/2 - (double)wav->data[2*i+1]/(double)65355*height();
                y2 = height()/2 - (double)wav->data[2*(i+1)+1]/(double)65355*height();

                painter.drawLine(x1, y1, x2, y2);
            }
        }
    }
    else
    {
        unsigned int sampleCount = wav->Subchunk2Size / 2;

        double scale = (double)(sampleCount - 1)/(double)width();

        double x1, x2, y1, y2;

        if(scale > 2)
        {
            painter.setRenderHint(QPainter::Antialiasing, false);

            for(int i = 0; i < width(); i++)
            {
                x1 = i;
                y1 = height()/2 - (double)getMin(wav, i*(double)scale, (i+1)*scale, 1)/(double)65355*height();
                x2 = i;
                y2 = height()/2 - (double)getMax(wav, i*(double)scale, (i+1)*scale, 1)/(double)65355*height();

                painter.drawLine(x1, y1, x2, y2);
            }
        }
        else
        {
            painter.setRenderHint(QPainter::Antialiasing, true);

            for(unsigned int i = 0; i < sampleCount - 1; i++)
            {
                x1 = (double)i/scale;
                y1 = height()/2 - (double)wav->data[i]/(double)65355*height();
                x2 = (double)(i+1)/scale;
                y2 = height()/2 - (double)wav->data[i+1]/(double)65355*height();

                painter.drawLine(x1, y1, x2, y2);
            }
        }
    }
}

void RenderArea::plotEnvelope(Envelope *env)
{
    QPainter painter(this);
    QPen wavePen = QPen(QColor(31, 255, 127, 255));
    painter.setPen(wavePen);

    double scale = (double)(env->samples - 1)/(double)width();

    double x1, x2, y1, y2;

    if(scale > 2)
    {
        painter.setRenderHint(QPainter::Antialiasing, false);

        for(int i = 0; i < width(); i++)
        {
            x1 = i;
            y1 = height()/2 + (double)getMax(env, i*(double)scale, (i+1)*scale)/(double)65355*height();
            x2 = i;
            y2 = height()/2 - (double)getMax(env, i*(double)scale, (i+1)*scale)/(double)65355*height();

            painter.drawLine(x1, y1, x2, y2);
        }
    }
    else
    {
        painter.setRenderHint(QPainter::Antialiasing, true);

        for(unsigned int i = 0; i < env->samples - 1; i++)
        {
            x1 = (double)i/scale;
            y1 = height()/2 - (double)env->data[i]/(double)65355*height();
            x2 = (double)(i+1)/scale;
            y2 = height()/2 - (double)env->data[i+1]/(double)65355*height();

            painter.drawLine(x1, y1, x2, y2);
        }
    }
}

void RenderArea::plotStaticChar(GainComputer *g)
{
    QPainter painter(this);

    double scale = (double)(96)/(double)width();

    painter.setRenderHint(QPainter::Antialiasing, true);

    QPointF p1, p2;

    QPen charPen = QPen(QColor(63, 63, 63, 63));
    charPen.setWidth(0);
    painter.setPen(charPen);



    for(int i = 12; i < 96; i = i + 12)
    {
        p1.rx() = 0;
        p1.ry() = (float)i/96*height();
        p2.rx() = width();
        p2.ry() = (float)i/96*height();
        painter.drawLine(p1, p2);

        p1.rx() = (float)i/96*width();
        p1.ry() = 0;
        p2.rx() = (float)i/96*width();
        p2.ry() = height();
        painter.drawLine(p1, p2);
    }

    charPen.setColor(QColor(255, 141, 141, 205));
    painter.setPen(charPen);

    painter.drawLine((96.33 + g->getThreshold() - g->getKneeWidth()/2)*width()/96.33, 0, (96.33 + g->getThreshold() - g->getKneeWidth()/2)*width()/96.33, height());
    painter.drawLine((96.33 + g->getThreshold() + g->getKneeWidth()/2)*width()/96.33, 0, (96.33 + g->getThreshold() + g->getKneeWidth()/2)*width()/96.33, height());

    charPen.setColor(QColor(63, 255, 63, 255));
    charPen.setWidth(1);
    painter.setPen(charPen);

    painter.drawLine((96.33 + g->getThreshold())/96.33*width(), 0, (96.33 + g->getThreshold())/96.33*width(), height());

    charPen.setWidth(3);
    charPen.setColor(QColor(63, 171, 255, 255));
    painter.setPen(charPen);

    for(unsigned int i = 0; i < 96; i++)
    {
        p1.rx() = (double)i/scale;
        p1.ry() = ( (-1) * (double)g->gainFunction((double)g->identity->data[i]))/(double)96*height();
        p2.rx() = (double)(i+1)/scale;
        p2.ry() = ( (-1) * (double)g->gainFunction((double)g->identity->data[i+1]))/(double)96*height();

        painter.drawLine(p1, p2);
    }

    charPen.setWidth(1);
    charPen.setColor(QColor(0, 0, 0, 255));
    painter.setPen(charPen);

    for(int i = 6; i < 96; i = i + 6)
    {
        p1.rx() = 2;
        p1.ry() = (float)i/96*height() - 12;

        painter.drawStaticText(p1, QStaticText("-" + QString::number(i)));

        p1.rx() = width() - (float)i/96*width();
        p1.ry() = height() - 16;

        painter.drawStaticText(p1, QStaticText("-" + QString::number(i)));
    }
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    if(env1 != nullptr)
    {
        //plotEnvelope(env1);
    }

    if(sound != nullptr)
    {
        plotWave(sound);
    }

    if(gainComputer != nullptr)
    {
        plotStaticChar(gainComputer);
    }

    QPainter paint(this);

    pen = QPen(Qt::black);
    paint.setPen(pen);
    paint.setBrush(brush);
    paint.drawRect(QRect(0, 0, width() - 1, height() - 1));
}
