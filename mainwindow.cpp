#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    plot1 = new RenderArea;
    plot2 = new RenderArea;

    thresEdit = new QLineEdit();
    ratioEdit = new QLineEdit();
    kneeEdit = new QLineEdit();

    thresLabel = new QLabel(tr("&Threshold:"));
    ratioLabel = new QLabel(tr("&Ratio:"));
    kneeLabel = new QLabel(tr("&Knee Width:"));

    thresLabel->setBuddy(thresEdit);
    ratioLabel->setBuddy(ratioEdit);
    kneeLabel->setBuddy(kneeEdit);

    thresEdit->setFixedSize(64, 20);
    ratioEdit->setFixedSize(64, 20);
    kneeEdit->setFixedSize(64, 20);

    thresEdit->setAlignment(Qt::AlignRight);
    ratioEdit->setAlignment(Qt::AlignRight);
    kneeEdit->setAlignment(Qt::AlignRight);

    QGridLayout *gcControlLayout = new QGridLayout;

    gcControlLayout->addWidget(thresLabel, 0, 0);
    gcControlLayout->addWidget(ratioLabel, 0, 1);
    gcControlLayout->addWidget(kneeLabel, 0, 2);
    gcControlLayout->addWidget(thresEdit, 1, 0);
    gcControlLayout->addWidget(ratioEdit, 1, 1);
    gcControlLayout->addWidget(kneeEdit, 1, 2);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(5, 5, 5, 5);

    gridLayout->addWidget(topFiller, 0, 0, 1, 3);
    gridLayout->addLayout(gcControlLayout, 1, 0);
    gridLayout->addWidget(plot1, 2, 0, 1, 2);
    gridLayout->addWidget(plot2, 2, 2, 1, 1);

    plot2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    widget->setLayout(gridLayout);

    createActions();
    createMenus();

    connect(thresEdit, &QLineEdit::editingFinished, this, &MainWindow::thresChanged);
    connect(ratioEdit, &QLineEdit::editingFinished, this, &MainWindow::ratioChanged);
    connect(kneeEdit, &QLineEdit::editingFinished, this, &MainWindow::kneeChanged);

    setMinimumSize(plot2->minimumSizeHint().height() * 2, plot2->minimumSizeHint().height() + 40 + topFiller->minimumSizeHint().height());
    resize(1280, plot2->minimumSizeHint().height() + 40 + topFiller->minimumSizeHint().height());

    audio = new wave();

    mainGC = new GainComputer();
    plot2->gainComputer = mainGC;

    thresEdit->setText("-48");
    ratioEdit->setText("4");
    kneeEdit->setText("12");

    thresChanged();
    ratioChanged();
    kneeChanged();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
}

void MainWindow::open()
{
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this, tr("Load a file"), "", tr("WAVE audio file (*.wav);;All Files (*)"));

    if(filePath != "")
    {
        audio->load(filePath);

        QFile *f = new QFile(filePath);
        QFileInfo fileInfo(f->fileName());
        QString filename(fileInfo.fileName());
        this->setWindowTitle(filename);

        plot1->sound = audio;
        plot1->update();

        runSidechain();
    }
}

void MainWindow::thresChanged()
{
    if((thresEdit->text().toFloat() < -96.33001) || (thresEdit->text().toFloat() > 0))
    {
        thresEdit->blockSignals(true);

        QMessageBox messageBox;

        messageBox.critical(0,"Error", "Valid threshold values range from -96.33 to 0");
        messageBox.setFixedSize(500,200);

        thresEdit->setText(*new QString(""));

        thresEdit->blockSignals(false);

        return;
    }

    mainGC->setThreshold(thresEdit->text().toFloat());
    runSidechain();
}

void MainWindow::ratioChanged()
{
    if(ratioEdit->text().toFloat() <= 0)
    {
        ratioEdit->blockSignals(true);

        QMessageBox messageBox;

        messageBox.critical(0,"Error", "Ratio must be greater than 0");
        messageBox.setFixedSize(500,200);

        ratioEdit->setText(*new QString(""));

        ratioEdit->blockSignals(false);

        return;
    }

    mainGC->setRatio(ratioEdit->text().toFloat());
    runSidechain();
}

void MainWindow::kneeChanged()
{
    if((kneeEdit->text().toFloat() < 0) || (kneeEdit->text().toFloat() > 96.33001))
    {
        kneeEdit->blockSignals(true);

        QMessageBox messageBox;

        messageBox.critical(0,"Error", "Valid knee width values range from 0 to 96.33");
        messageBox.setFixedSize(500,200);

        kneeEdit->setText(*new QString(""));

        kneeEdit->blockSignals(false);

        return;
    }

    mainGC->setKneeWidth(kneeEdit->text().toFloat());
    runSidechain();
}

void MainWindow::runSidechain()
{
    if(audio == nullptr || audio->loaded == false)
    {
        plot2->update();
        return;
    }

    plot1->sound = audio;

    wave* mono;

    if(audio->NumChannels == 2)
    {
        mono = new wave();
        mono->mergeStereo(audio);
    }
    else
    {
        //mono = new wave(audio);
        mono = audio;
    }

    env1 = new Envelope(mono->Subchunk2Size / 2);
    for(int i = 0; i < mono->Subchunk2Size / 2; i++)
    {
        env1->data[i] = qFabs(mono->data[i]);
    }

    plot1->env1 = this->env1;

    plot1->update();
    plot2->update();
}
