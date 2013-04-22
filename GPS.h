#ifndef GPS_H
#define GPS_H

#include "mainwindow.h"
#include "QString"

////////////////////////////////////////////////
///Открытие порта
////////////////////////////////////////////////
void MainWindow::startSlave(QString portName)
{

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

/////////////////////////////////////////
///Обработка ошибок
/////////////////////////////////////////
void MainWindow::processError(const QString &s)
{
    ui->gps_status->setText("GPS: error");
    qDebug()<<  s;
}

///////////////////////////////////////////
///функция захвата и конвертации NMEA с GPS
///////////////////////////////////////////
void  MainWindow::readRequest(){
 if (!timer.isActive()) timer.start(50);
        request.append(serial.readAll());
        qDebug()<<"///"+request;

 if (request.size() >= 71 && request.size()<=75 ) {
    qDebug()<<request;
    //далее начинаем парсить строку от указателя $, если таакой имеется
    if (request[0] == '$')
        {
         buffer=request;
         QStringList gps_data = buffer.split(",");
         if (gps_data[0] == "$GPRMC" && gps_data[2] == "A") {

         qDebug()<<gps_data;

         if(gps_data[11][1]=='*'){
             //конвертируем мили/ч км/ч
             double speed=gps_data[7].toDouble();
             speed=speed/0.5399614;
             gps_data[7]=gps_data[7].setNum(speed);

             //конвертируем градусы в минуты
             gps_data[5]= convertGRAD(gps_data[6],gps_data[5]);
             gps_data[3]="0"+gps_data[3];
             gps_data[3]= convertGRAD(gps_data[4],gps_data[3]);

             //отправляем данные в БД
             connector.addGPScordinaes(gps_data[5],gps_data[3], gps_data[7],
                                       gps_data[8],"false",idmain);
             //перерисовываем маркер
             layer->latitude = gps_data[5].toDouble();
             layer->longitude = gps_data[3].toDouble();
             layer->course = gps_data[8].toDouble();
             }
         }
    }
   request.clear();
   }
}

void MainWindow::processTimeout()
{
   request.clear();
}


////////////////////////////////////////////////
///Конвертируем минуты в градусы
////////////////////////////////////////////////
QString MainWindow::convertGRAD(QString flag, QString cord)
{
    QString latgr;
   //генирируем градусную состовляющую
    latgr[0]=cord[0];    latgr[1]=cord[1];
    latgr[2]=cord[2];

    QString min, converted;
    //генирируем минутную в градусною состовляющую
    min[0]=cord[3];    min[1]=cord[4];
    min[2]='.';        min[3]=cord[6];
    min[4]=cord[7];    min[5]=cord[8];
    min[6]=cord[9];

    double convmin= min.toDouble();
    convmin=convmin/60;
    min=min.setNum(convmin);
    converted=latgr+".";
    //доклеиваем строку
    for (int mn = 2;  mn < min.length(); mn++)
    {
        converted[mn+2]=min[mn];
    }

    //если вдруг мы окажемся в другом полушарии...
    if (flag=="S" || flag=="W") { converted ="-" + converted;}
    qDebug()<<converted;

    return converted;

}

#endif // GPS_H
