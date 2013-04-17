#include "database.h"
#include "client.h"

DataBase::DataBase(QObject *parent) :
    QObject(parent)
{
}

 QSqlDatabase db;


bool DataBase::activateConnection()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("GPSdata");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setUserName("postgres");

    if (!db.open()) {
        qDebug() << QObject::trUtf8("Error") << db.lastError().text();
        return -1;
    }

    return true;

}

void DataBase::closeConnection(){
    db.close();
}

void DataBase::addGPScordinaes(QString lat,QString lon, QString time, QString date,  QString speed, QString course, QString sendflag, QString id)
{
    QSqlQuery query;
    QString dateCV =convertDate(time,date);

    query.exec("insert into coordinates values('"+dateCV+"',"+speed+","+course+","+sendflag+","+id+",'"+lat+"', '"+lon+"')");
    rpcclient.sendGPScordinaes(lat,lon,dateCV,speed,course,id);
}
QString DataBase::convertDate(QString time, QString date)
{
    QString res,timeConv, dateConv ;
    timeConv[0] = time[0];
    timeConv[1] = time[1];
    timeConv[2] = ':';
    timeConv[3] = time[2];
    timeConv[4] = time[3];
    timeConv[5] = ':';
    timeConv[6] = time[4];
    timeConv[7] = time[5];

    dateConv[0]='2';
    dateConv[1]='0';
    dateConv[2]=date[4];
    dateConv[3]=date[5];
    dateConv[4]='-';
    dateConv[5]=date[2];
    dateConv[6]=date[3];
    dateConv[7]='-';
    dateConv[8]=date[0];
    dateConv[9]=date[1];

    //dateConv = "20"+date[4]+time[5]+'-'+time[2]+time[3]+'-'+time[0]+time[1];
    res = dateConv+' '+timeConv;
    qDebug()<<res;
    return res;
}
