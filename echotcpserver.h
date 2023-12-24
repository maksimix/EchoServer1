#ifndef ECHOTCPSERVER_H
#define ECHOTCPSERVER_H

#include <QObject>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QDebug>


class EchoTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit EchoTcpServer(QObject *parent = nullptr);

public slots:
    void slotServerReadClient();
    void slotNewConnection();
    void slotClientDisconnected();

private:
    QTcpServer * eTcpServer;

    int server_status;
    QMap<int, QTcpSocket *> SClients;

signals:

};

#endif // ECHOTCPSERVER_H
