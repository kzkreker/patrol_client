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
    void keyPressEvent(QKeyEvent* e);

private:
    void processError(const QString &s);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *nam;

    QSerialPort serial;
    QByteArray request;
    QTimer timer;
};

#endif // MAINWINDOW_H
