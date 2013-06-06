#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QByteArray>
#include <QKeyEvent>
#include <marble/MarbleWidget.h>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <math.h>
#include "mypaintlayer.h"
#include <QObject>
#include <QByteArray>

#include "database.h"
#include "ui_mainwindow.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


signals:
 void downloaded();

private slots:

    //слоты управления шасси
    void forvard();
    void back();
    void leftUp();
    void leftDw();
    void rightUp();
    void rightDw();
    void strafeL();
    void strafeR();
    void stop();

    // работа с последовательным портом
    void startSlave(QString portName);
    void readRequest();
    void processTimeout();
    //прочее
    QString convertGRAD(QString flag, QString cord);
    void keyPressEvent(QKeyEvent* e);
    QString getDataTime();


    //работа с переферийным контроллером
    void getStatusXML();
    void StatusDownloaded(QNetworkReply* pReply);
    void parseXML();
    QMap<QString, QString> parseLine(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml,
                                QMap<QString, QString>& map) const;

    void addLinesToList(QList< QMap<QString,QString> >& persons);

    // функция вызова общей отправки
    void sendALL();

private:
    void processError(const QString &s);
    MyPaintLayer* layer;
    Ui::MainWindow *ui;
    QNetworkAccessManager *nam;
    QSerialPort serial;
    QTimer timer;
    //объект для работы с БД
    DataBase connector;
    QString idmain;

    QString buffer;
    QString request;
    QString m_DownloadedData;
    QString portname;
    QString statusURL;

    QStringList cordinatesGPS;
    QStringList picVaList;

};

#endif // MAINWINDOW_H

