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
    plot3 = new RenderArea;

    thresEdit = new QLineEdit();
    ratioEdit = new QLineEdit();
    kneeEdit = new QLineEdit();
    zoomEdit = new QLineEdit();
    cursorEdit = new QLineEdit();
    attackEdit = new QLineEdit();
    releaseEdit = new QLineEdit();
    gainEdit = new QLineEdit();

    scCheck = new QCheckBox("Enable soft clipping", this);

    thresLabel = new QLabel(tr("&Threshold [dB]:"));
    ratioLabel = new QLabel(tr("&Ratio:"));
    kneeLabel = new QLabel(tr("&Knee Width [dB]:"));
    zoomLabel = new QLabel(tr("&Zoom [%]:"));
    cursorLabel = new QLabel(tr("&Cursor [Samples]:"));
    attackLabel = new QLabel(tr("&Attack [ms]"));
    releaseLabel = new QLabel(tr("&Release [ms]"));
    gainLabel = new QLabel(tr("&Gain [dB]"));

    thresLabel->setBuddy(thresEdit);
    ratioLabel->setBuddy(ratioEdit);
    kneeLabel->setBuddy(kneeEdit);
    zoomLabel->setBuddy(zoomEdit);
    cursorLabel->setBuddy(cursorEdit);
    attackLabel->setBuddy(attackEdit);
    releaseLabel->setBuddy(releaseEdit);
    gainLabel->setBuddy(gainEdit);

    thresEdit->setFixedSize(72, 20);
    ratioEdit->setFixedSize(64, 20);
    kneeEdit->setFixedSize(80, 20);
    zoomEdit->setFixedSize(64, 20);
    cursorEdit->setFixedSize(84, 20);
    attackEdit->setFixedSize(72, 20);
    releaseEdit->setFixedSize(64, 20);
    gainEdit->setFixedSize(80, 20);

    thresEdit->setAlignment(Qt::AlignRight);
    ratioEdit->setAlignment(Qt::AlignRight);
    kneeEdit->setAlignment(Qt::AlignRight);
    zoomEdit->setAlignment(Qt::AlignRight);
    cursorEdit->setAlignment(Qt::AlignRight);
    attackEdit->setAlignment(Qt::AlignRight);
    releaseEdit->setAlignment(Qt::AlignRight);
    gainEdit->setAlignment(Qt::AlignRight);

    QGridLayout *controlLayout = new QGridLayout;
    QGridLayout *gcControlLayout = new QGridLayout;
    QGridLayout *uiControlLayout = new QGridLayout;

    controlLayout->addLayout(gcControlLayout, 0, 0);
    controlLayout->addLayout(uiControlLayout, 1, 0);

    gcControlLayout->addWidget(thresLabel, 0, 0);
    gcControlLayout->addWidget(ratioLabel, 0, 1);
    gcControlLayout->addWidget(kneeLabel, 0, 2);
    gcControlLayout->addWidget(thresEdit, 1, 0);
    gcControlLayout->addWidget(ratioEdit, 1, 1);
    gcControlLayout->addWidget(kneeEdit, 1, 2);
    gcControlLayout->addWidget(attackLabel, 2, 0);
    gcControlLayout->addWidget(releaseLabel, 2, 1);
    gcControlLayout->addWidget(gainLabel, 2, 2);
    gcControlLayout->addWidget(attackEdit, 3, 0);
    gcControlLayout->addWidget(releaseEdit, 3, 1);
    gcControlLayout->addWidget(gainEdit, 3, 2);
    gcControlLayout->addWidget(scCheck, 4, 0, 1, 2);

    uiControlLayout->addWidget(zoomLabel, 0, 0);
    uiControlLayout->addWidget(cursorLabel, 0, 1);
    uiControlLayout->addWidget(zoomEdit, 1, 0);
    uiControlLayout->addWidget(cursorEdit, 1, 1);

    gcControlLayout->setAlignment(Qt::AlignTop);
    gcControlLayout->setHorizontalSpacing(48);
    uiControlLayout->setAlignment(Qt::AlignBottom);
    uiControlLayout->setHorizontalSpacing(56);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(5, 5, 5, 5);

    gridLayout->addWidget(topFiller, 0, 0, 1, 3);
    gridLayout->addWidget(plot1, 1, 0, 1, 2);
    gridLayout->addLayout(controlLayout, 1, 2);
    gridLayout->addWidget(plot3, 2, 0, 1, 2);
    gridLayout->addWidget(plot2, 2, 2, 1, 1);

    plot2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    widget->setLayout(gridLayout);

    createActions();
    createMenus();

    connect(thresEdit, &QLineEdit::editingFinished, this, &MainWindow::thresChanged);
    connect(ratioEdit, &QLineEdit::editingFinished, this, &MainWindow::ratioChanged);
    connect(kneeEdit, &QLineEdit::editingFinished, this, &MainWindow::kneeChanged);
    connect(zoomEdit, &QLineEdit::editingFinished, this, &MainWindow::zoomChanged);
    connect(cursorEdit, &QLineEdit::editingFinished, this, &MainWindow::cursorChanged);
    connect(attackEdit, &QLineEdit::editingFinished, this, &MainWindow::attackChanged);
    connect(releaseEdit, &QLineEdit::editingFinished, this, &MainWindow::releaseChanged);
    connect(gainEdit, &QLineEdit::editingFinished, this, &MainWindow::gainChanged);
    connect(scCheck, &QCheckBox::stateChanged, this, &MainWindow::scChanged);

    setMinimumSize(plot2->minimumSizeHint().height() * 2, plot2->minimumSizeHint().height() + 40 + topFiller->minimumSizeHint().height());
    resize(1440, plot2->minimumSizeHint().height()*2 + 40 + topFiller->minimumSizeHint().height());

    audio = new wave();
    output = nullptr;
    playback = nullptr;

    mainGC = new GainComputer();
    softClipper = new GainComputer();

    plot2->gainComputer = mainGC;
    plot3->drawParam = mainGC;

    softClipper->setThreshold(0);
    softClipper->setRatio(1000000);
    softClipper->setKneeWidth(12);

    mainLD = new LevelDetector();

    thresEdit->setText("-24");
    ratioEdit->setText("4");
    kneeEdit->setText("12");
    zoomEdit->setText("100");
    cursorEdit->setText("0");
    attackEdit->setText("10");
    releaseEdit->setText("100");
    gainEdit->setText("0");

    scCheck->setCheckState(Qt::Checked);

    cursor = 0;
    zoom = 1;

    thresChanged();
    ratioChanged();
    kneeChanged();
    zoomChanged();
    cursorChanged();
    attackChanged();
    releaseChanged();
    gainChanged();
    scChanged();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open"), this);
    saveAsAct = new QAction(tr("&Save as"), this);

    openAct->setShortcuts(QKeySequence::Open);
    saveAsAct->setShortcuts(QKeySequence::Save);

    openAct->setStatusTip(tr("Open an existing file"));
    saveAsAct->setStatusTip(tr("Save as a new file"));

    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsAct);
}

void MainWindow::updateCursor()
{
    plot1->cursor = this->cursor;
    plot3->cursor = this->cursor;

    return;
}

void MainWindow::updateZoom()
{
    plot1->zoom = this->zoom;
    plot3->zoom = this->zoom;
}

void MainWindow::open()
{
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this, tr("Load a file"), "", tr("WAVE audio file (*.wav);;All Files (*)"));

    if(filePath != "")
    {
        audio->load(filePath);

        if(audio->loaded == false || audio == nullptr)
        {
            return;
        }

        output = new wave(audio, 0);
        playback = output;

        QFile *f = new QFile(filePath);
        QFileInfo fileInfo(f->fileName());
        QString filename(fileInfo.fileName());
        this->setWindowTitle(filename);

        wave* mono;

        if(audio->NumChannels == 2)
        {
            mono = new wave();
            mono->mergeStereo(audio);
        }
        else
        {
            mono = audio;
        }

        inputEnv = new Envelope(mono->Subchunk2Size / 2);
        inputEnv->setFS(mono->SampleRate);

        for(int i = 0; i < mono->Subchunk2Size / 2; i++)
        {
            if(mono->data[i] == 0)
                inputEnv->data[i] = -96.33;
            else
                inputEnv->data[i] = 20*qLn(qFabs(mono->data[i])/32768)/qLn(10);
        }

        env1 = new Envelope(mono->Subchunk2Size / 2);
        env1->setFS(mono->SampleRate);
        env1->setColor(255, 167, 38, 255);

        zoomEdit->setText("100");
        zoomChanged();

        plot1->sound = audio;
        plot1->sound_out = output;

        runSidechain();
    }
}

void MainWindow::saveAs()
{
    if(output != nullptr)
    {
        QString filePath;
        filePath = QFileDialog::getSaveFileName(this, tr("Save output"), "", tr("WAVE audio file (*.wav);;All Files (*)"));

        if(filePath != "")
        {
            output->store(filePath);
        }
    }
    else
    {
        QMessageBox messageBox;

        messageBox.critical(0,"Error", "No output signal present");
        messageBox.setFixedSize(500,200);

        return;
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

    return;
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

    return;
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

    return;
}

void MainWindow::attackChanged()
{
    mainLD->setAttack(attackEdit->text().toDouble());
    attackEdit->setText(QString::number(mainLD->getAttack()));

    runSidechain();
}

void MainWindow::releaseChanged()
{
    mainLD->setRelease(releaseEdit->text().toDouble());
    releaseEdit->setText(QString::number(mainLD->getRelease()));

    runSidechain();
}

void MainWindow::gainChanged()
{
    mainGC->setMakeupGain(gainEdit->text().toDouble());
    runSidechain();
}

void MainWindow::scChanged()
{
    if(scCheck->checkState() == Qt::Checked)
    {
        scEnabled = true;
    }
    else if(scCheck->checkState() == Qt::Unchecked)
    {
        scEnabled = false;
    }

    runSidechain();
}

void MainWindow::zoomChanged()
{
    if(zoomEdit->text().toFloat() < 100)
    {
        zoomEdit->blockSignals(true);

        QMessageBox messageBox;

        messageBox.critical(0,"Error", "Zoom must be at least than 100%");
        messageBox.setFixedSize(500,200);

        zoomEdit->setText(*new QString("100"));
        zoomChanged();

        zoomEdit->blockSignals(false);

        return;
    }

    if((plot1 != nullptr) && (plot3 != nullptr) && (audio->loaded == true))
    {
        if(audio->loaded != false)
        {
            double z = zoom / (this->zoomEdit->text().toFloat()/100);
            double zoomWindow = audio->Subchunk2Size/(2 * audio->NumChannels)/zoom;
            zoom = this->zoomEdit->text().toFloat()/100;

            cursor += (1-z)/2 * zoomWindow;

            if(cursor < 0)
                cursor = 0;
            else if(cursor + z * zoomWindow >= audio->Subchunk2Size/(2 * audio->NumChannels))
            {
                cursor = audio->Subchunk2Size/(2 * audio->NumChannels) - z * zoomWindow;
            }
        }

        cursorEdit->setText(QString::number(cursor));

        updateZoom();
        updateCursor();

        plot1->update();
        plot3->update();
    }

    return;
}

void MainWindow::cursorChanged()
{
    if(audio->loaded != false)
    {
        if((cursorEdit->text().toInt() < 0) || (cursorEdit->text().toInt() >= audio->Subchunk2Size / (2 * audio->NumChannels)))
        {
            cursorEdit->blockSignals(true);

            QMessageBox messageBox;

            messageBox.critical(0,"Error", "Cursor must be a valid sample index");
            messageBox.setFixedSize(500,200);

            cursorEdit->setText(*new QString("0"));
            cursor = 0;
            updateCursor();

            cursorEdit->blockSignals(false);

            return;
        }
        else
        {
            if((cursorEdit->text().toInt() + ((audio->Subchunk2Size / (2 * audio->NumChannels)) / zoom)) < (audio->Subchunk2Size / (2 * audio->NumChannels) - 1))
            {
                cursor = cursorEdit->text().toInt();

            }
            else
            {
                cursor = audio->Subchunk2Size / (2 * audio->NumChannels) - audio->Subchunk2Size / (2 * audio->NumChannels)/zoom;

                cursorEdit->setText(QString::number(cursor));
            }

            updateCursor();
            plot1->update();
            plot3->update();

            return;
        }
    }
}

void MainWindow::transformView(QWheelEvent *event)
{
    int totalSamples = audio->Subchunk2Size/(2 * audio->NumChannels);
    double zoomIncrement = 0;
    double displayedSamples0 = totalSamples/zoom;
    double cursorIncrement = 0;

    if(event->delta() > 0)
        zoomIncrement = zoom/3;
    else if(event->delta() < 0)
        zoomIncrement = (-1) * zoom/4;

    zoom += zoomIncrement;

    if(zoom < 1)
        zoom = 1;

    double displayedSamples1 = totalSamples/zoom;

    cursorIncrement = ((double)event->x() - (double)plot1->x())/(double)plot1->width() * (displayedSamples0 - displayedSamples1);

    if(qFabs(cursorIncrement) - qFloor(qFabs(cursorIncrement)) > 0.5)
        if(cursorIncrement > 0)
            cursor += qCeil(cursorIncrement);
        else
            cursor += qFloor(cursorIncrement);
    else
        if(cursorIncrement > 0)
            cursor += qFloor(cursorIncrement);
        else
            cursor += qCeil(cursorIncrement);

    if(cursor < 0)
        cursor = 0;
    if(cursor > audio->Subchunk2Size/(2 * audio->NumChannels) - displayedSamples1)
        cursor = audio->Subchunk2Size/(2 * audio->NumChannels) - displayedSamples1;

    zoomEdit->setText(QString::number((int)(zoom*100)));
    cursorEdit->setText(QString::number(cursor));

    plot1->transform(zoom, cursor);
    plot3->transform(zoom, cursor);
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(event->x() > plot1->x() && event->y() > plot1->y() && event->x() < (plot3->x() + plot3->width() - 1) && event->y() < (plot3->y() + plot3->height() - 1) && audio->loaded != false)
    {
        transformView(event);
    }
}

void MainWindow::runSidechain()
{
    if(audio == nullptr || audio->loaded == false)
    {
        plot2->update();
        plot3->update();
        return;
    }

    for(int i = 0; i < inputEnv->samples; i++)
    {
        env1->data[i] = mainGC->gainFunction(inputEnv->data[i]) - inputEnv->data[i];
    }

    mainLD->filter(env1, env1);

    if(audio->NumChannels == 2)
    {
        double y1, y2, buff1, buff2, buff3, buff4;

        for(int i = 0; i < output->Subchunk2Size/2; i = i + 2)
        {
            if(scEnabled == true)
            {
                buff1 = (double)audio->data[i] * pow(10, (env1->data[i/2]+mainGC->getMakeupGain())/20);
                buff2 = (double)audio->data[i+1] * pow(10, (env1->data[i/2]+mainGC->getMakeupGain())/20);

                if(buff1 == 0)
                {
                    buff3 = -96.33;
                }
                else
                {
                    buff3 = 20*qLn(qFabs(buff1)/32768)/qLn(10);
                }
                if(buff2 == 0)
                {
                    buff4 = -96.33;
                }
                else
                {
                    buff4 = 20*qLn(qFabs(buff2)/32768)/qLn(10);
                }

                y1 = buff1 * pow(10, (softClipper->gainFunction(buff3) - buff3)/20);
                y2 = buff2 * pow(10, (softClipper->gainFunction(buff4) - buff4)/20);
            }
            else
            {
                y1 = audio->data[i] * pow(10, (env1->data[i/2]+mainGC->getMakeupGain())/20);
                y2 = audio->data[i+1] * pow(10, (env1->data[i/2]+mainGC->getMakeupGain())/20);
            }

            if(qFabs(y1) > 32767)
            {
                if(y1 < 0)
                    y1 = -32768;
                else
                    y1 = 32767;
            }
            if(qFabs(y2) > 32767)
            {
                if(y2 < 0)
                    y2 = -32768;
                else
                    y2 = 32767;
            }

            output->data[i] = y1;
            output->data[i+1] = y2;
        }
    }
    else
    {
        double y, buff1, buff2;

        for(int i = 0; i < output->Subchunk2Size/2; i = i + 1)
        {
            if(scEnabled == true)
            {
                buff1 = (double)audio->data[i] * pow(10, (env1->data[i] + mainGC->getMakeupGain())/20);

                if(buff1 == 0)
                {
                    buff2 = -96.33;
                }
                else
                {
                    buff2 = 20*qLn(qFabs(buff1)/32768)/qLn(10);
                }

                y = buff1 * pow(10, (softClipper->gainFunction(buff2) - buff2)/20);
            }
            else
            {
                y = audio->data[i] * (double)pow(10, (env1->data[i] + mainGC->getMakeupGain())/20);
            }

            if(qFabs(y) > 32767)
            {
                if(y < 0)
                    y = -32768;
                else
                    y = 32767;
            }

            output->data[i] = y;
        }
    }


    plot3->env1 = this->inputEnv;
    plot3->env2 = this->env1;

    plot1->update();
    plot2->update();
    plot3->update();
}
