#ifndef RELAY_H
#define RELAY_H
#include "mainwindow.h"

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

#endif // RELAY_H
