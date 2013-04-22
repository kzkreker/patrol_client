#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
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

#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
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

private:
    void processError(const QString &s);
    MyPaintLayer* layer;
    Ui::MainWindow *ui;
    QNetworkAccessManager *nam;
    QSerialPort serial;
    QTimer timer;

    DataBase connector;
    QString idmain;
    QString buffer;
    QString request;
};

#endif // MAINWINDOW_H

