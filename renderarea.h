#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPainter>
#include <QtMath>
#include <QPointF>
#include <QStaticText>

#include <wave.h>
#include <envelope.h>
#include <gaincomputer.h>

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    explicit RenderArea(QWidget *parent = 0);

    enum plotType {waveform, envelope, staticChar};

    wave *sound; 
    Envelope *env1;
    GainComputer *gainComputer;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    int getMax(wave *audio, unsigned int start, unsigned int end, int increment);
    int getMax(Envelope *env, unsigned int start, unsigned int end);

    int getMin(wave *audio, unsigned int start, unsigned int end, int increment);

    void plot(wave *audio, QPainter *paint);
    void plotWave(wave *sound);
    void plotEnvelope(Envelope *env);
    void plotStaticChar(GainComputer *g);

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:

    QPen pen;
    QBrush brush;

};

#endif // RENDERAREA_H
