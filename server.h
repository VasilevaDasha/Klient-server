#pragma once
#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>

class Server:public QWebSocketServer
{
    Q_OBJECT; //нужен чтобы использовать сигналы и слоты
public:
    explicit Server(QObject *parent = nullptr);
    ~Server(); //деструктор, в котором происходит очистка памяти, выделенной на куче для клиентских сокетов и объекта потока
private:
    QList<QWebSocket*> clientsSocks; 
    void stopWaitingLoop(); //ожидание команды для остановки сервера
    std::thread *m_thread;
public slots: 
    void clientConnected(); //слот для приема сигнала о соединении
    void messageReceived(const QString &msg);
    void clientDisconnected(); //слот для приема сигнала об отключении клиента
};

