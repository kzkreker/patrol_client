#include "client.h"

Client::Client(QObject* parent) : QObject(parent) {
//	rpc = new MaiaXmlRpcClient(QUrl("http://phpxmlrpc.sourceforge.net/server.php"), this);
//	rpc = new MaiaXmlRpcClient(QUrl("https://rpc.gandi.net/xmlrpc/2.0/"), this);
    rpc = new MaiaXmlRpcClient(QUrl("http://dataBase:8082/RPC2"), this);

    QSslConfiguration config = rpc->sslConfiguration();
    config.setProtocol(QSsl::AnyProtocol);
    rpc->setSslConfiguration(config);

    connect(rpc, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            this, SLOT(handleSslErrors(QNetworkReply *, const QList<QSslError> &)));
 }

bool Client::sendGPScordinaes(QString lat, QString lon, QString dateCV, QString speed, QString course, QString id) {
    QVariantList args;

    args.clear();
    args<<lat;
    args<<lon;
    args<<dateCV;
    args<<speed;
    args<<course;
    args<<id;

    rpc->call("transport.curentGPSResiv", args,
                this, SLOT(curentGPSSendResponse(QVariant &)),
                this, SLOT(testFault(int, const QString &)));

}


void Client::testResponse(QVariant &arg) {
        qDebug() << arg.toString();
}

void Client::testFault(int error, const QString &message) {
        qDebug() << "EEE:" << error << "-" << message;
}

void Client::towelResponse(QVariant &arg) {
    qDebug() << "Next years Towel Day is on" << arg.toDateTime();
}

void Client::curentGPSSendResponse(QVariant &arg) {
    qDebug() << arg;
}
void Client::handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
    qDebug() << "SSL Error:" << errors;
    reply->ignoreSslErrors(); // don't do this in real code! Fix your certs!
}

