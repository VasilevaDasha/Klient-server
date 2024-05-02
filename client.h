#pragma once
#include <QWebSocket>
#include <QObject>
#include <QThread>
#include <QTextStream>

class Client: public QObject
{
    Q_OBJECT //нужен чтобы использовать сигналы и слоты

public:
    explicit Client(QObject *parent = nullptr);
    ~Client(); //деструктор, в котором происходит очистка памяти на куче, выделенной под клиентский сокет и объекта потока
signals:
    void needToSendMessage(const QString msg); //сигнал отправляемый объектом об отправке сообщения
public slots:
    void serverMessageReceived(const QByteArray &bytes); //слот для приема сигнала о сообщении
    void messageReceived(const QString &msg);
private:
    void messageWaitingLoop(); //ожидает сообщение для остановки сервера
    void connectToServer(const QPair<QString,QString> connectionData); 
    QWebSocket *socket;
    std::thread *m_thread;
};
