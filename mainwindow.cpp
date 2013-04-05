#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

#include <QtSerialPort/QSerialPortInfo>



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

 }


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
//           QMessageBox* box = new QMessageBox();
//           box->setWindowTitle(QString("Hello"));
//           box->setText(QString("You Pressed: ")+ e->text());
//           box->show();

           switch( e->text().toInt() ) {

           case 8:
               forvard();
           break;

           case 2:
               back();
           break;

           case 4:
               strafeL();
           break;

           case 6:
               strafeR();
           break;

           case 7:
               leftUp();
           break;

           case 1:
               leftDw();
           break;

           case 9:
               rightUp();
           break;

           case 3:
               rightDw();
           break;

           case 5:
               stop();
           break;
           }
}
void MainWindow::stop()
{
    QString url = "http://192.168.1.66/leds.cgi?led=7";
    nam->get(QNetworkRequest(QUrl(url)));

}
void MainWindow::forvard()
{
    QString url = "http://192.168.1.66/leds.cgi?led=5";
    nam->get(QNetworkRequest(QUrl(url)));

}
void MainWindow::back()
{
    QString url = "http://192.168.1.66/leds.cgi?led=9";
    nam->get(QNetworkRequest(QUrl(url)));

}
void MainWindow::rightUp()
{
    QString url = "http://192.168.1.66/leds.cgi?led=1";
    nam->get(QNetworkRequest(QUrl(url)));

}
void MainWindow::rightDw()
{
    QString url = "http://192.168.1.66/leds.cgi?led=2";
    nam->get(QNetworkRequest(QUrl(url)));

}
void MainWindow::leftUp()
{
    QString url = "http://192.168.1.66/leds.cgi?led=3";
    nam->get(QNetworkRequest(QUrl(url)));

}
void MainWindow::leftDw()
{
    QString url = "http://192.168.1.66/leds.cgi?led=4";
    nam->get(QNetworkRequest(QUrl(url)));

}
void MainWindow::strafeL()
{
    QString url = "http://192.168.1.66/leds.cgi?led=6";
    nam->get(QNetworkRequest(QUrl(url)));

}
void MainWindow::strafeR()
{
    QString url = "http://192.168.1.66/leds.cgi?led=8";
    nam->get(QNetworkRequest(QUrl(url)));

}

void MainWindow::startSlave()
{
   QString portName="ttyUSB0";

    if (serial.portName() != portName) {
        serial.close();
        serial.setPortName(portName);

        if (!serial.open(QIODevice::ReadWrite)) {
            processError(tr("Can't open %1, error code %2")
                         .arg(serial.portName()).arg(serial.error()));
            return;
        }

        if (!serial.setBaudRate(QSerialPort::Baud9600)) {
            processError(tr("Can't set baud rate 9600 baud to port %1, error code %2")
                         .arg(serial.portName()).arg(serial.error()));
            return;
        }

        if (!serial.setDataBits(QSerialPort::Data8)) {
            processError(tr("Can't set 8 data bits to port %1, error code %2")
                         .arg(serial.portName()).arg(serial.error()));
            return;
        }

        if (!serial.setParity(QSerialPort::NoParity)) {
            processError(tr("Can't set no patity to port %1, error code %2")
                         .arg(serial.portName()).arg(serial.error()));
            return;
        }

        if (!serial.setStopBits(QSerialPort::OneStop)) {
            processError(tr("Can't set 1 stop bit to port %1, error code %2")
                         .arg(serial.portName()).arg(serial.error()));
            return;
        }

        if (!serial.setFlowControl(QSerialPort::NoFlowControl)) {
            processError(tr("Can't set no flow control to port %1, error code %2")
                         .arg(serial.portName()).arg(serial.error()));
            return;
        }
    }

    ui->gps_status->setText("GPS: ON");
}

void MainWindow::processError(const QString &s)
{
    ui->gps_status->setText("GPS: error");
    qDebug()<<  s;
}

  QString buffer;
  QString request;

void  MainWindow::readRequest()
{

  request.append(serial.readAll());


  if (request.size() > 70) {


//if (request[request.size()-1] == '/n')
if (request[0] == '$')
  {
    buffer=request;
  QStringList gps_data = buffer.split(",");
  if (gps_data[0] == "$GPRMC" && gps_data[2] == "A") {
 // c.insert(gps_collection, gps2bson(gps_data));
   qDebug()<<gps_data;
  }

    }
  request.clear();
  }
}
