#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPainter>
#include <QtMath>
#include <QPointF>
#include <QStaticText>
#include <QLinearGradient>

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
    Envelope *env2;
    GainComputer *gainComputer;
    GainComputer *drawParam;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    int getMax(wave *audio, unsigned int start, unsigned int end, int increment);
    double getMax(Envelope *env, unsigned int start, unsigned int end);

    int getMin(wave *audio, unsigned int start, unsigned int end, int increment);
    double getMin(Envelope *env, unsigned int start, unsigned int end);

    void plot(wave *audio, QPainter *paint);
    void plotWave(wave *sound);
    void plotEnvelope(Envelope *env, bool reversed, bool fill);
    void plotStaticChar(GainComputer *g);
    void transform(double zoom, int cursor);

    int cursor;
    double zoom;

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:

    QPen pen;
    QBrush brush;

};

#endif // RENDERAREA_H
