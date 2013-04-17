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
    
signals:
    
public slots:
    void addGPScordinaes(QString lat,QString lon, QString time, QString date,
                         QString speed, QString course, QString sendflag, QString id);

    void closeConnection();
    //void readSettings();
    //void addSettings();
    bool activateConnection();
    QString convertDate(QString, QString);
    private:
    Client rpcclient;
};

#endif // DATABASE_H
