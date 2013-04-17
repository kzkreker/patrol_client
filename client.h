#ifndef CLIENT_H
#define CLIENT_H

#include "./maiaXmlRpcClient.h"

class Client : public QObject {
    Q_OBJECT

    public:
        Client(QObject* parent = 0);

    public slots:
        bool sendGPScordinaes(QString lat,QString lon, QString dateCV,
                              QString speed, QString course, QString id);
    private slots:
        void testResponse(QVariant &);
        void testFault(int, const QString &);
        void towelResponse(QVariant &);
        void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
        void curentGPSSendResponse(QVariant &);

    private:
        MaiaXmlRpcClient *rpc;
};

#endif
