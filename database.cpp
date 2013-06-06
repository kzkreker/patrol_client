#include "database.h"
#include "client.h"
#include "mainwindow.h"

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

standartTables();
    connect(&rpcclient,SIGNAL(resivOKNow()),this,SLOT(updateSended()));
    connect(&rpcclient,SIGNAL(resivOKNot()),this,SLOT(updateNotSended()));



    return true;
}


void DataBase::closeConnection(){
    db.close();
}

/////////////////////////////////////////////////
///Отправка данных в БД и на удаленный сервер
/////////////////////////////////////////////////
void DataBase::sendGPSPICNow(QStringList GPS, QStringList PIC)
{

    QSqlQuery query;
    dataTrans = getDateTime();

    if (GPS.size()==0)
    {GPS.append("fail");}

    if (PIC.size()==0)
    {PIC.append("fail");}

    //отправка кординат в таблицу GPS
    if (GPS[0]!="fail")
    {
    query.exec("insert into vehicle_"+idmain+" values('"+ dataTrans+"',"+GPS[2]+","+GPS[3]+","
                      +"'false'"+","+idmain+",'"+GPS[0]+"', '"+GPS[1]+"')");}

    //отправка показаний датчиков в таблицу PIC
    if (PIC[0]!="fail")
    {
    query.exec( "INSERT INTO vehicle_pic_"+idmain+" (an, led, pot, temp, id, data,send)"+
                "VALUES ('{"+PIC[0]+"}', '{"+PIC[1]+"}', "+PIC[2]+","+PIC[3]+", "+idmain+", '"+ dataTrans+"','false')");
    }

    //отправка кординат и показаний датчиков на удвленный сервер
    rpcclient.sendNow(GPS,PIC,idmain,dataTrans);
    // ждем и молимся

}

/////////////////////////////////////////////////////
///Изменение записи как отправленной
/////////////////////////////////////////////////////
void DataBase::sendGPSPICNot()
{
 QSqlQuery query;

 QString resoult;
 QString NotSendPIC;
 QString NotSendGPS;
 //сбрасываем списки отправленных
 NotSendGPSTime.clear();
 NotSendPICTime.clear();
 qDebug()<<dataTrans;

   query.exec("SELECT * FROM  public.vehicle_pic_"+idmain+"  WHERE vehicle_pic_"+idmain+".send = FALSE "+
              "AND vehicle_pic_"+idmain+".data < '"+dataTrans+"' LIMIT 10");

   while (query.next())
      {
         resoult= query.record().value(0).toString()+"~";
         resoult= resoult+query.record().value(1).toString()+"~";
         resoult= resoult+query.record().value(2).toString()+"~";
         resoult= resoult+query.record().value(3).toString()+"~";
         resoult= resoult+query.record().value(5).toString();
         NotSendPIC = NotSendPIC+resoult+"|";
         NotSendPICTime.append(query.record().value(5).toString());
      }

    qDebug()<<NotSendPIC;
    qDebug()<<NotSendPICTime;

    query.exec("SELECT * FROM  public.vehicle_"+idmain+"  WHERE vehicle_"+idmain+".sflag = FALSE "
               "AND vehicle_"+idmain+".data < '"+dataTrans+"' LIMIT 10");

    while (query.next())
       {
           resoult= query.record().value(0).toString()+"~";
           resoult= resoult+query.record().value(1).toString()+"~";
           resoult= resoult+query.record().value(2).toString()+"~";
           resoult= resoult+query.record().value(5).toString()+"~";
           resoult= resoult+query.record().value(6).toString();
           NotSendGPS = NotSendGPS+resoult+"|";
           NotSendGPSTime.append(query.record().value(0).toString());
       }

     qDebug()<<NotSendGPS;
     qDebug()<<NotSendGPSTime;

     //отсылка кординат
     if(NotSendGPS!="" || NotSendPIC!="" ){
     rpcclient.sendNot(NotSendPIC,NotSendGPS,idmain);}
}

/////////////////////////////////////////////////////
///Отправка сообщения
/////////////////////////////////////////////////////

void DataBase::sendMessage()
{

}

//////////////////////////////////////////////
///Обновление не отправленных данных в таблице
//////////////////////////////////////////////
void DataBase::updateSended()
{
    QSqlQuery query;

    qDebug()<<"updateSended";

    //обновим две отправленые записи
    query.exec( "UPDATE vehicle_"+idmain+" SET sflag=true WHERE data='"+dataTrans+"' AND id ="+idmain);
    query.exec( "UPDATE vehicle_pic_"+idmain+" SET send=true WHERE data='"+dataTrans+"' AND id ="+idmain);

    //вызов функции отправки не отправленных данных
    sendGPSPICNot();
}

//////////////////////////////////////////////
///Обновление не отправленных данных в таблице
//////////////////////////////////////////////
void DataBase::updateNotSended()
{
 QSqlQuery query;
 qDebug()<<"updateNotSended";

 foreach(QString time, NotSendGPSTime){
 query.exec( "UPDATE vehicle_"+idmain+" SET sflag=true WHERE data='"+time+"' AND id ="+idmain);}

 foreach(QString time, NotSendPICTime){
 query.exec( "UPDATE vehicle_pic_"+idmain+" SET send=true WHERE data='"+time+"' AND id ="+idmain);}
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

///////////////////////////////////////////////////////
///создаем стандартные таблицы
///////////////////////////////////////////////////////

void DataBase::standartTables()
{
//запрос создает таблицу для хранения кординат транспорта
 QSqlQuery query;
 query.exec("CREATE TABLE IF NOT EXISTS  vehicle_"+ idmain + " ( data timestamp without time zone NOT NULL,"+
            "speed double precision NOT NULL, course double precision,  sflag boolean,  id integer NOT NULL,"+
            "lat character varying(12), lon character varying(12), CONSTRAINT id_key"+ idmain +" PRIMARY KEY (id , data ))"+
            "WITH ( OIDS=FALSE); ALTER TABLE vehicle_"+ idmain +" OWNER TO postgres;");


 query.exec("CREATE TABLE IF NOT EXISTS  vehicle_pic_"+idmain+" ( an integer[], led integer[], "+
            "pot integer, temp integer, id integer NOT NULL, data timestamp without time zone NOT NULL, send boolean, "+
            "CONSTRAINT id_prim"+idmain+" PRIMARY KEY (id , data )) WITH (OIDS=FALSE ); ALTER TABLE vehicle_pic_"+idmain+
            " OWNER TO postgres");
}
