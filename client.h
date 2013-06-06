#ifndef CLIENT_H
#define CLIENT_H

#include "./maiaXmlRpcClient.h"

class Client : public QObject {
    Q_OBJECT

    public:
        Client(QObject* parent = 0);
    signals:
        void resivOKNow();
        void resivOKNot();
        void networkFail();
       void networkUP();
    public slots:
        bool sendNow(QStringList PIC, QStringList GPS, QString idmain,QString dataCV);
        bool sendNot(QString PIC, QString GPS, QString idmain);
    private slots:

        void testFault(int, const QString &);

        //
        void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

        //обрабатываем успешную передачу
        void curentGPSPICSendResponse(QVariant &);
        void sendNotGPSPICSendResponse(QVariant &);

    private:
        MaiaXmlRpcClient *rpc;
        QString idmaincl;
};

#endif
