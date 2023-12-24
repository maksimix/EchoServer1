# Эхо сервер на C++
## Основные функции данного сервера:

### Конструктор класса **EchoTcpServer**
```cpp
EchoTcpServer::EchoTcpServer(QObject *parent) : QObject(parent)
{
 eTcpServer = new QTcpServer(this); // инициализация обьекта класса QTcpServer

 connect(eTcpServer, &QTcpServer::newConnection, this, &EchoTcpServer::slotNewConnection); // устанавливаем соединение  между сигналом newConnection объекта eTcpServer и слотом slotNewConnection текущего объекта EchoTcpServer.

// Попытка запустить сервер на всех доступных адресах и порту 6000
 if(!eTcpServer->listen(QHostAddress::Any,6000)){  
    // Если не удалось запустить сервер, выводим сообщение об ошибке
    qDebug() << "Сервер не включен"; 
 }else {
    // Если сервер успешно запущен, выводим сообщение о успешном включении
    qDebug() << "Сервер включен";
 }

 // Выводим доступные IP адреса
 const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
 for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
     if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
          qDebug() << address.toString();
 }

}

```

### Функция ***slotNewConnection*** 
SlotNewConnection является слотом, который связан с сигналом newConnection экземпляра QTcpServer . Эта функция вызывается каждый раз, когда новый клиент подключается к серверу.
 ```cpp
 void EchoTcpServer::slotNewConnection(){


    // Вывод в консоль о новом соединении  
    qDebug() << QString::fromUtf8("Соединение"); 
    
    //Получает указатель на объект QTcpSocket, представляющий подключенного клиента.
    QTcpSocket* clientSocket = eTcpServer->nextPendingConnection(); 
   
    //Получает дескриптор сокета клиента
    int idUserSoc = clientSocket ->socketDescriptor(); 
    // Добавляет клиента в QMap SClients
    SClients[idUserSoc] = clientSocket; 

    //Устанавливает соединение сигнала readyRead сокета клиента с слотом slotServerReadClient класса EchoTcpServer.
    connect(SClients[idUserSoc], &QTcpSocket::readyRead, this, &EchoTcpServer::slotServerReadClient); 

    //Устанавливает соединение сигнала disconnected сокета клиента с слотом slotClientDisconnected класса EchoTcpServer
    connect(SClients[idUserSoc], &QTcpSocket::disconnected, this, &EchoTcpServer::slotClientDisconnected); 

}
 ```
## Функция  ***slotServerReadClient***
***slotServerReadClient*** - слот класса **EchoTcpServer*.

Этот слот вызывается при наличии данных для чтения от клиента.
```cpp
void EchoTcpServer::slotServerReadClient(){

//Получает указатель на объект QTcpSocket, который отправил сигнал, и вызвала эту функцию.
        QTcpSocket* clientSocket = (QTcpSocket*)sender();

//читает все доступные данные из клиентского сокета и сохраняет их в массив байт (QByteArray) с именем array.
        QByteArray array  = clientSocket->readAll();

//Цикл перебирает все клиентские сокеты в карту SClients и отправляет эхо сигнал
        foreach (QTcpSocket* socket, SClients) { // 
            QTextStream os(socket);
            os.setAutoDetectUnicode(true);
            if(array.size()>0)
            os << array.constData() << "\n";
        }
//Вывыод данных в консоль 
    qDebug() << array <<"\n"<<"\n";

}
```


## Функция ***slotClientDisconnected***

 ***slotClientDisconnected*** - слот  класса **EchoTcpServer**. Этот слот вызывается при отключении клиента от сервера. 
```cpp
void EchoTcpServer::slotClientDisconnected(){
    //указатель на объект QTcpSocket, который отправил сигнал и вызвал эту функцию
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    //Получается дескриптор сокета клиента. 
    //Дескриптор сокета был использован как ключ при добавлении клиентского сокета в  SClients при подключении.
    int idusersocs=clientSocket->socketDescriptor();
    
    //Удаляет запись о клиентском сокете из карты SClients с использованием его дескриптора в качестве ключа. 
    SClients.remove(idusersocs);

}

```