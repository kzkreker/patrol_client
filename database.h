 #ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QObject>
#include <QDebug>
#include <QDate>
#include "client.h"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    QString idmain;
signals:
    
public slots:

    //основные рабочие функции
    void sendGPSPICNow(QStringList GPS, QStringList PIC);

    void sendGPSPICNot();
    void sendMessage();

    void updateNotSended();
    void updateSended();
    //закрытие подключения
    void closeConnection();
    //void readSettings();
    //void addSettings();

    //обслуживание
    void  standartTables();
    bool  activateConnection();

    QString getDateTime();
    private:
    Client rpcclient;
    QString  dataTrans;
    QStringList NotSendGPSTime;
    QStringList NotSendPICTime;

};

#endif // DATABASE_H
