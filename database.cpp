#include "database.h"
#include "client.h"

DataBase::DataBase(QObject *parent) :
    QObject(parent)
{
}

 QSqlDatabase db;


bool DataBase::activateConnection(QString id)
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

   //запрос создает таблицу для хранения кординат транспорта
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS  vehicle_"+ id + " ( data timestamp without time zone NOT NULL,"+
               "speed double precision NOT NULL, course double precision,  sflag boolean,  id integer NOT NULL,"+
               "lat character varying(12), lon character varying(12), CONSTRAINT id_key"+ id +" PRIMARY KEY (id , data ))"+
               "WITH ( OIDS=FALSE); ALTER TABLE vehicle_"+ id +" OWNER TO postgres;");
    return true;

}

void DataBase::closeConnection(){
    db.close();
}

/////////////////////////////////////////////////
///Отправка данных в БД и на удаленный сервер
/////////////////////////////////////////////////
void DataBase::addGPScordinaes(QString lat,QString lon, QString speed, QString course, QString sendflag, QString id)
{
    QSqlQuery query;
    QString dateCV =getDateTime();

    //отправка кординат
    query.exec("insert into coordinates values('"+dateCV+"',"+speed+","+course+","
               +sendflag+","+id+",'"+lat+"', '"+lon+"')");

    //отправка кординат на сервер
    rpcclient.sendGPScordinaes(lat,lon,dateCV,speed,course,id);
}

/////////////////////////////////////////////////////
///Получаем текущую дату с ПК
/////////////////////////////////////////////////////
QString DataBase::getDateTime()
{
    QDateTime Datatime= QDateTime::currentDateTime();
    QString res=  Datatime.toString("yyyy-MM-dd hh:mm:ss");

    qDebug()<<res;
    return res;
}
