#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QBoxLayout>
#include <QtMath>
#include <QLineEdit>
#include <QLabel>

#include <wave.h>
#include <renderarea.h>
#include <gaincomputer.h>
#include <envelope.h>

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

    wave *audio;
    Envelope *env1;
    GainComputer *mainGC;


private slots:
    void open();

    void thresChanged();
    void ratioChanged();
    void kneeChanged();

private:
    Ui::MainWindow *ui;

    void createActions();
    void createMenus();

    QMenu *fileMenu;

    QAction *openAct;

    QLineEdit *thresEdit;
    QLineEdit *ratioEdit;
    QLineEdit *kneeEdit;

    QLabel *thresLabel;
    QLabel *ratioLabel;
    QLabel *kneeLabel;
};
#endif // MAINWINDOW_H
