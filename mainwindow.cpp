#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "relay.h"
#include "GPS.h"

using namespace Marble;

QTimer seconds;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //управление шасси
    nam = new QNetworkAccessManager(this);
    ui->setupUi(this);

    connect(ui->forwardButton,SIGNAL(pressed()),this,SLOT(forvard()));
    connect(ui->forwardButton,SIGNAL(released()),this,SLOT(stop()));

    connect(ui->backButton,SIGNAL(pressed()),this,SLOT(back()));
    connect(ui->backButton,SIGNAL(released()),this,SLOT(stop()));

    connect(ui->rightUpButton,SIGNAL(pressed()),this,SLOT(rightUp()));
    connect(ui->rightUpButton,SIGNAL(released()),this,SLOT(stop()));

    connect(ui->rightDwButton,SIGNAL(pressed()),this,SLOT(rightDw()));
    connect(ui->rightDwButton,SIGNAL(released()),this,SLOT(stop()));

    connect(ui->leftUpButton,SIGNAL(pressed()),this,SLOT(leftUp()));
    connect(ui->leftUpButton,SIGNAL(released()),this,SLOT(stop()));

    connect(ui->leftDwButton,SIGNAL(pressed()),this,SLOT(leftDw()));
    connect(ui->leftDwButton,SIGNAL(released()),this,SLOT(stop()));

    connect(ui->strafeLButton,SIGNAL(pressed()),this,SLOT(strafeL()));
    connect(ui->strafeLButton,SIGNAL(released()),this,SLOT(stop()));

    connect(ui->strafeRButton,SIGNAL(pressed()),this,SLOT(strafeR()));
    connect(ui->strafeRButton,SIGNAL(released()),this,SLOT(stop()));
    //вывод доступных портов
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        ui->gpsname_comboBox->addItem(info.portName());

    //чтение порта
    connect(&serial, SIGNAL(readyRead()),this, SLOT(readRequest()));
    startSlave();

    connector.activateConnection();

    layer = new MyPaintLayer(ui->MarbleWidget);
    ui->MarbleWidget->addLayer(layer);
    ui->MarbleWidget->installEventFilter(layer);

    seconds.setInterval(1000);

    QObject::connect(&seconds, SIGNAL(timeout()), ui->MarbleWidget, SLOT(update()));
    seconds.start();

 }


MainWindow::~MainWindow()
{
    delete ui;
}



