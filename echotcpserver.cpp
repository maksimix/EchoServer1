#include "echotcpserver.h"

EchoTcpServer::EchoTcpServer(QObject *parent) : QObject(parent)
{
 eTcpServer = new QTcpServer(this);

 connect(eTcpServer, &QTcpServer::newConnection, this, &EchoTcpServer::slotNewConnection);

 if(!eTcpServer->listen(QHostAddress::Any,6000)){
    qDebug() << "Сервер не включен";
 }else {
     qDebug() << "Сервер включен";
 }
 const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
 for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
     if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
          qDebug() << address.toString();
 }

}


void EchoTcpServer::slotNewConnection(){



    qDebug() << QString::fromUtf8("Соединение");
    QTcpSocket* clientSocket = eTcpServer->nextPendingConnection();
    int idUserSoc = clientSocket ->socketDescriptor();
    SClients[idUserSoc] = clientSocket;


    connect(SClients[idUserSoc], &QTcpSocket::readyRead, this, &EchoTcpServer::slotServerReadClient);
    connect(SClients[idUserSoc], &QTcpSocket::disconnected, this, &EchoTcpServer::slotClientDisconnected);


}



void EchoTcpServer::slotServerReadClient(){


        QTcpSocket* clientSocket = (QTcpSocket*)sender();
        QByteArray array  = clientSocket->readAll();


        foreach (QTcpSocket* socket, SClients) {
            QTextStream os(socket);
            os.setAutoDetectUnicode(true);
            if(array.size()>0)
            os << array.constData() << "\n";
        }

    qDebug() << array <<"\n"<<"\n";

}


void EchoTcpServer::slotClientDisconnected(){
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int idusersocs=clientSocket->socketDescriptor();
    SClients.remove(idusersocs);

}
