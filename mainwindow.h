#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QByteArray>
#include <QKeyEvent>

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

    void keyPressEvent(QKeyEvent* e);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *nam;
};

#endif // MAINWINDOW_H
