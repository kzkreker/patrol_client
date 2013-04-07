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

    void forvard();
    void back();
    void leftUp();
    void leftDw();
    void rightUp();
    void rightDw();
    void strafeL();
    void strafeR();
    void stop();
    void startSlave();
    void readRequest();
    QString convertGRAD(QString flag, QString cord);
    void keyPressEvent(QKeyEvent* e);

private:
    void processError(const QString &s);
    MyPaintLayer* layer;
private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *nam;

    QSerialPort serial;
    QByteArray request;
    QTimer timer;
    DataBase connector;
};

#endif // MAINWINDOW_H
