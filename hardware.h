#ifndef HARDWARE_H
#define HARDWARE_H

#include "mainwindow.h"
#include <QDebug>

void MainWindow::sendALL()
{
connector.sendGPSPICNow(cordinatesGPS, picVaList);
}

#endif // HARDWARE_H
