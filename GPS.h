#ifndef GPS_H
#define GPS_H

#include "mainwindow.h"
#include "QString"

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

void  MainWindow::readRequest(){

    request.append(serial.readAll());

if (request.size() >= 70) {
    qDebug()<<request;
    if (request[0] == '$')
        {
         buffer=request;
         QStringList gps_data = buffer.split(",");
         if (gps_data[0] == "$GPRMC" && gps_data[2] == "A") {
         // c.insert(gps_collection, gps2bson(gps_data));
         qDebug()<<gps_data;
         if(gps_data[11][1]=='*'){
             double speed=gps_data[8].toDouble();
             speed=speed/0.5399614;
             gps_data[8]=gps_data[8].setNum(speed);



             gps_data[5]= convertGRAD(gps_data[6],gps_data[5]);
             gps_data[3]="0"+gps_data[3];
             gps_data[3]= convertGRAD(gps_data[4],gps_data[3]);


             connector.addGPScordinaes(gps_data[5],gps_data[3],gps_data[1],gps_data[9],gps_data[8],gps_data[10],"false","0");

             layer->latitude = gps_data[5].toDouble();
             layer->longitude = gps_data[3].toDouble();

             }
         }

    }
  request.clear();
  }
}

QString MainWindow::convertGRAD(QString flag, QString cord)
{
    QString latgr;


    latgr[0]=cord[0];    latgr[1]=cord[1];
    latgr[2]=cord[2];

    QString min, converted;

    min[0]=cord[3];    min[1]=cord[4];
    min[2]='.';        min[3]=cord[6];
    min[4]=cord[7];    min[5]=cord[8];
    min[6]=cord[9];

    double convmin= min.toDouble();
    convmin=convmin/60;
    min=min.setNum(convmin);
    converted=latgr+".";

for (int mn = 2;  mn < min.length(); mn++)
{
    converted[mn+2]=min[mn];
}

    if (flag=="S" || flag=="W") { converted ="-" + converted;}
    qDebug()<<converted;
    return converted;

}

#endif // GPS_H
