#include "server.h"
#include <QDebug>
#include <QTextStream>
#include <QHostInfo>
QTextStream cin(stdin);

Server::Server(QObject *parent) 
    : QWebSocketServer{"EchoServer",QWebSocketServer::NonSecureMode,parent}
{
    if(this->listen(QHostAddress::AnyIPv4,5555)){ //формирование списка адресов и их вывод вместе с портом для подключения
        QString ipAddress ="";
        QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
        QList<QHostAddress> listAddr= info.addresses();
        for( int i = 0; i < listAddr.size(); i ++ ){
            if( listAddr.at(i).protocol() == QAbstractSocket::IPv4Protocol ){
                ipAddress += " | ";
                ipAddress += listAddr.at(i).toString();
            }
        }
        qDebug() << "Server started successfully"
                 << "for stop please enter \"stop\" \n"
                 << "Allowed addresses:" << ipAddress
                 << "\n Allowed ports: " << this->serverPort();
    }
    else {
        qDebug() << this->errorString();
    }
    connect(this,&Server::newConnection,this, &Server::clientConnected); //позволяет определять и обрабатывать входящие соединения
    m_thread=new std::thread(std::bind(&Server::stopWaitingLoop,this));
}

Server::~Server() { //происходит очистка памяти, выделенной на куче для клиентских сокетов и объекта потока
    if(m_thread){
        delete m_thread;
    }
    for(auto &clientSock:clientsSocks){
        if(clientSock!=nullptr){
            clientSock->deleteLater();
        }
    }
    clientsSocks.clear();
}

void Server::stopWaitingLoop() //бесконечный цикл ожидающий ввода соответствующей команды для остановки сервера
{
    QString input;
    while(true){
        cin.readLineInto(&input,0);
        if(!input.isEmpty()){
            if(input=="stop" or input == "STOP" or input == "Stop"){
                break;
            }
        }
    }
    this->deleteLater();
    this->close();
}

void Server::clientConnected(){ //обеспечивает оповещение клиента о его успешном подключении

    QWebSocket *clientSock;
    clientSock = this->nextPendingConnection();
    clientSock->sendBinaryMessage(QByteArray::fromStdString(std::string("connected")));

    //связывает сигналы клиентского сокета с соответствующими слотами:
    connect(clientSock,&QWebSocket::textMessageReceived,this,&Server::messageReceived);
    connect(clientSock,&QWebSocket::disconnected,this,&Server::clientDisconnected);
    connect(clientSock,&QWebSocket::disconnected,clientSock,&QWebSocket::deleteLater);

    //заносит указатель на клиентский сокет в список
    clientsSocks.push_back(clientSock);
    qDebug() << "Client with address:" << clientSock->peerAddress().toString()
             << "has been connected";
}

void Server::messageReceived(const QString &msg){ //Выводит в консоль адрес и сообщение клиента, затем рассылает эту пару всем подключенным пользователям
    QWebSocket *clientSenderSock=(QWebSocket*)sender();
    qDebug() << "Message from: "<< clientSenderSock->peerAddress().toString() << " is: " << msg;
    for(auto &clientSock: clientsSocks){
        if(clientSock->peerAddress()!=clientSenderSock->peerAddress()){
            clientSock->sendTextMessage(clientSenderSock->peerAddress().toString()+QString(":")+msg);
        }
    }
}

void Server::clientDisconnected() //отвечает за вывод сообщения об отключении клиента и очистку памяти, выделенной для его сокета
{
    QWebSocket *p_disconnectedClientSock= qobject_cast<QWebSocket *>(sender());
    qDebug() << "Client with address:" << p_disconnectedClientSock->peerAddress().toString()
             << "has been disconnected";
    auto disconnectedClientPos=std::find(clientsSocks.begin(),clientsSocks.end(),p_disconnectedClientSock);
    clientsSocks.at(std::distance(clientsSocks.begin(),disconnectedClientPos))->deleteLater();
    clientsSocks.erase(disconnectedClientPos);
}


