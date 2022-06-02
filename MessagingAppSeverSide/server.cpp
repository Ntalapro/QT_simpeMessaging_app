#include "server.h"
#include <QAbstractSocket>


server::server(QObject*)
{

}

void server::startServer(){

    allClients = new QVector<QTcpSocket*>;//vector of clients

    chatServer = new QTcpServer();//create chatserver
    chatServer->setMaxPendingConnections(10);//set a limit of client connections to the server
    QAbstractSocket::connect(chatServer,&QTcpServer::newConnection,this
                             ,&server::newClientConnection);//connect server to this object, and signal newCo
                                                            //to slot newCient

    if(chatServer->listen(QHostAddress::Any,8001)){

        qDebug() << "Sever has Started. Listening to port 8001.";//if server started
    }
    else{
         qDebug() << "Sever failed to start. Error: " + chatServer->errorString();
    }

}

void server:: newClientConnection(){

    QTcpSocket* client = chatServer->nextPendingConnection(); // obtain socket from sever(via nextPendingCon)
    QString ipAddress = client->peerAddress().toString(); // connected client's ip
    int port = client->peerPort();//connected client's port

    //connect signal changes to slots
    QAbstractSocket::connect(client,&QTcpSocket::disconnected,this,&server::socketDisconnected);
    QAbstractSocket::connect(client,&QTcpSocket::readyRead,this,&server::socketReadyRead);
    QAbstractSocket::connect(client,&QTcpSocket::stateChanged,this,&server::socketStateChanged);

    allClients->push_back(client);//push client to allClient vector

    qDebug() << "Socket connected from " + ipAddress + ": " + QString::number(port);

}

void server::socketDisconnected(){

    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender()); //pointer to obj of disconnceted signal
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();

    qDebug() << "Socket is disconnected from " + socketIpAddress + ": "+QString::number(port);
}

void server::socketReadyRead(){
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());//convert the obj  to TcpSocket
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();

    QString data = QString(client->readAll());//so we can use readAll()

    qDebug() <<"Message: " + data + " (" +socketIpAddress + ": " +QString::number(port) + ")";
    sendMessageToClients(data);
}

void server:: socketStateChanged(QAbstractSocket::SocketState state){

    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();

    QString desc;
    //keep track of the states
    if(state == QAbstractSocket::UnconnectedState)
        desc = "The socket is not connected.";
    else if (state == QAbstractSocket::HostLookupState)
        desc= "The socket is performing a host name lookup.";
    else if (state == QAbstractSocket::ConnectingState)
        desc ="The socket has started establishing a connection.";
    else if (state == QAbstractSocket::ConnectedState)
        desc = "A conection is established.";
    else if (state == QAbstractSocket::BoundState)
        desc ="The socket is bound to an addres and port.";
    else if (state == QAbstractSocket::ClosingState)
        desc = "The socket is about to close (data ma still be waiting to be written).";
    else if (state == QAbstractSocket::ListeningState)
        desc = "For internal use only.";

    qDebug() << "Socket state changee=d  (" +socketIpAddress+ ": " + QString::number(port) + ") : " + desc;
}

void server:: sendMessageToClients(QString message){

    //loop through all clients and pass message data to all connected clients.
    if(allClients ->size() >0){
        for(int i=0;i<allClients->size();i++){
           if(allClients->at(i)->isOpen() && allClients->at(i)->isWritable()){
               allClients->at(i)->write(message.toUtf8());
           }
        }
    }

}





















