#include "mainwindow.h"
#include "relay.h"
#include "GPS.h"
#include "picweb.h"
#include "hardware.h"

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


    // гдето тут функция получения настроек
    idmain ="0";
    portname ="ttyUSB0";
    statusURL ="http://192.168.1.66/status.xml";
    connector.idmain=idmain;

    //чтение порта
    timer.setSingleShot(true);
    connect(&serial, SIGNAL(readyRead()),this, SLOT(readRequest()));
    connect(&timer, SIGNAL(timeout()),this, SLOT(processTimeout()));

    startSlave(portname);

    //создаем объект для работы с БД
    connector.activateConnection();

    //настройка элемента выбора даты архива
    ui->timeNow->setMinimumDate(QDate::currentDate().addYears(-1));
    ui->timeNow->setMaximumDate(QDate::currentDate().addYears(5));
    ui->timeNow->setMinimumTime(QTime::currentTime().addSecs(-3600));
    ui->timeNow->setMaximumTime(QTime::currentTime().addSecs(3600));
    ui->timeNow->setDateTime(QDateTime::currentDateTime());
    ui->timeNow->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    //рисуем на слоях глобуса
    layer = new MyPaintLayer(ui->MarbleWidget);
    ui->MarbleWidget->addLayer(layer);
    ui->MarbleWidget->installEventFilter(layer);

    //отсечка прорисовки
    seconds.setInterval(1000);

    QObject::connect(&seconds, SIGNAL(timeout()), ui->MarbleWidget, SLOT(update()));
    QObject::connect(&seconds, SIGNAL(timeout()), SLOT( getStatusXML()));
    QObject::connect(&seconds, SIGNAL(timeout()), SLOT( sendALL()));

    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)),
                     SLOT(StatusDownloaded(QNetworkReply*)));

    seconds.start();

 }


MainWindow::~MainWindow()
{
    delete ui;
}



