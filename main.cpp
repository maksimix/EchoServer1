#include <QCoreApplication>
#include "echotcpserver.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    EchoTcpServer server;



    return a.exec();
}
