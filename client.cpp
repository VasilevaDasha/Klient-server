#include "client.h"
#include <QLocale>
QTextStream cin(stdin);

Client::Client(QObject *parent) //происходит инициализация объекта клиентского сокета
    :QObject{parent}
{
    cin.setEncoding(QStringConverter::Encoding::System);
    socket=new QWebSocket;

    //связывание сигналов с соответствующими слотами:
    connect(socket,&QWebSocket::textMessageReceived,this,&Client::messageReceived);
    connect(socket,&QWebSocket::binaryMessageReceived,this,&Client::serverMessageReceived);
    connect(socket,&QWebSocket::disconnected,socket,&QWebSocket::deleteLater);
    connect(this,&Client::needToSendMessage,socket,&QWebSocket::sendTextMessage);

    //происходит чтение из консоли ip-адреса и порта для подключения
    QString addr,port;
    qDebug() << "Please, enter server address:";
    cin >> addr;
    qDebug() << "Please, enter port for connecting: ";
    cin >> port;
    connectToServer(qMakePair(addr,port));
}

void Client::connectToServer(const QPair<QString,QString> connectionData){ //происходит соединение с сервером, с указанием пришедших данных для подключения
    socket->open(QUrl(QString("ws://")+connectionData.first+QString(":")+connectionData.second));
}

void Client::messageWaitingLoop()
{
    QString message;
    while(true){
        cin.readLineInto(&message);
        if(!message.isEmpty()){
            qDebug() << "You:" << message;
            emit needToSendMessage(message);
        }
    }
}


Client::~Client() //происходит очистка памяти на куче, выделенной под клиентский сокет и объекта потока
{
    if(m_thread){
        delete m_thread;
    }
    socket->deleteLater();
}

void Client::messageReceived(const QString &msg){ //выводит на экран сообщения от других клиентов
   qDebug() << msg;
}


void Client::serverMessageReceived(const QByteArray &bytes){ //отвечает за обработку сообщения от сервера об успешном подключении и выводит соответствующую информацию в консоль
    QString str(bytes);
    if(str=="connected"){
        qDebug() << "Successfully connected to server."
                 << "Now you can enter your message";
    }
    m_thread = new std::thread(std::bind(&Client::messageWaitingLoop,this));
}


