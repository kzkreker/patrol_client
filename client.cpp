#include "client.h"
#include "mainwindow.h"
#include "database.h"

//////////////////////////////////////////////////////
///конструктор клиента
//////////////////////////////////////////////////////
Client::Client(QObject* parent) : QObject(parent) {
    //	rpc = new MaiaXmlRpcClient(QUrl("http://phpxmlrpc.sourceforge.net/server.php"), this);
    //	rpc = new MaiaXmlRpcClient(QUrl("https://rpc.gandi.net/xmlrpc/2.0/"), this);

    //вундервафля работает поверх апача, однако цепляемся к самодельному серверу
    rpc = new MaiaXmlRpcClient(QUrl("http://dataBase:8082/RPC2"), this);
    // конфиги
    QSslConfiguration config = rpc->sslConfiguration();
    config.setProtocol(QSsl::AnyProtocol);

    rpc->setSslConfiguration(config);

    //сигнал-слот обработка ошибок
    connect(rpc, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            this, SLOT(handleSslErrors(QNetworkReply *, const QList<QSslError> &)));

}

///////////////////////////////////////////////////
///Отправка кординат
///////////////////////////////////////////////////
bool Client::sendNow(QStringList GPS,QStringList PIC, QString idmain,QString dataCV) {

    QVariantList args;
    idmaincl=idmain;
    //перегоняем в кошерные строки
    QString PICstring;
    QString GPString;
     qDebug()<<"send";
     qDebug()<<PIC;
     qDebug()<<GPS;

    if (PIC[0]!="fail"){
    PICstring= PIC[0]+"|"+ PIC[1]+"|"+PIC[2]+"|"+PIC[3];} else {PICstring= PIC[0];}
    if (GPS[0]!="fail"){
    GPString= GPS[0]+"|"+ GPS[1]+"|"+GPS[2]+"|"+GPS[3];} else {GPString= GPS[0];}

    qDebug()<<PICstring;
    qDebug()<<GPString;
   //Заполняем список данными о текущем положении
    args.clear();
    args<<GPString;
    args<<PICstring;
    args<<idmain;
    args<<dataCV;
    //отправляем данные на сервер
    rpc->call("transport.curentGPSPICResiv", args,
                this, SLOT(curentGPSPICSendResponse(QVariant &)),
                this, SLOT(testFault(int, const QString &)));

}

bool Client::sendNot(QString PIC, QString GPS, QString idmain)
{
    QVariantList args;
    args<<GPS;
    args<<PIC;
    args<<idmain;

    rpc->call("transport.sendNotGPSPICResiv", args,
                this, SLOT(sendNotGPSPICSendResponse(QVariant &)),
                this, SLOT(testFault(int, const QString &)));
}

/////////////////////////////////////////////////////
///Ответ на отправку подтверждаем пересылку в БД
////////////////////////////////////////////////////
void Client::curentGPSPICSendResponse(QVariant &arg) {
    qDebug()<<"resiv";
    emit resivOKNow();
    emit networkUP();
}

void Client::sendNotGPSPICSendResponse(QVariant &arg)
{
    qDebug()<<"resiv";
    emit resivOKNot();
}

/////////////////////////////////////////////////////
///обработка сбоев
/////////////////////////////////////////////////////
void Client::testFault(int error, const QString &message) {
        qDebug() << "EEE:" << error << "-" << message;
        emit networkFail();
}

///////////////////////////////////////////////////
///обработка сбоев  SSL c
///////////////////////////////////////////////////

void Client::handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
    qDebug() << "SSL Error:" << errors;
    reply->ignoreSslErrors(); // don't do this in real code! Fix your certs!
}

