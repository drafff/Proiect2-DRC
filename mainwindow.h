#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QBoxLayout>
#include <QtMath>
#include <QLineEdit>
#include <QLabel>
#include <QWheelEvent>
#include <QPushButton>
#include <QAudioOutput>
#include <QCheckBox>
#include <QBuffer>
#include <QByteArray>
#include <QDebug>

#include <wave.h>
#include <renderarea.h>
#include <gaincomputer.h>
#include <envelope.h>
#include <leveldetector.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void runSidechain();

    QBoxLayout *plotsLayout;

    RenderArea *plot1;
    RenderArea *plot2;
    RenderArea *plot3;

    wave *audio;
    wave *output;
    wave *playback;
    Envelope *inputEnv;
    Envelope *env1;
    GainComputer *mainGC;
    GainComputer *softClipper;
    LevelDetector *mainLD;


private slots:
    void open();
    void saveAs();

    void thresChanged();
    void ratioChanged();
    void kneeChanged();
    void zoomChanged();
    void cursorChanged();
    void attackChanged();
    void releaseChanged();
    void gainChanged();
    void scChanged();

    void wheelEvent(QWheelEvent *event) override;

private:
    Ui::MainWindow *ui;

    void createActions();
    void createMenus();
    void updateCursor();
    void updateZoom();
    void transformView(QWheelEvent *event);

    QMenu *fileMenu;

    QAction *openAct;
    QAction *saveAsAct;

    QLineEdit *thresEdit;
    QLineEdit *ratioEdit;
    QLineEdit *kneeEdit;
    QLineEdit *zoomEdit;
    QLineEdit *cursorEdit;
    QLineEdit *attackEdit;
    QLineEdit *releaseEdit;
    QLineEdit *gainEdit;

    QCheckBox *scCheck;

    QLabel *thresLabel;
    QLabel *ratioLabel;
    QLabel *kneeLabel;
    QLabel *zoomLabel;
    QLabel *cursorLabel;
    QLabel *attackLabel;
    QLabel *releaseLabel;
    QLabel *gainLabel;

    int cursor;
    double zoom;
    bool scEnabled;
};
#endif // MAINWINDOW_H
