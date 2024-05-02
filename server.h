#pragma once
#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>

class Server:public QWebSocketServer
{
    Q_OBJECT; //����� ����� ������������ ������� � �����
public:
    explicit Server(QObject *parent = nullptr);
    ~Server(); //����������, � ������� ���������� ������� ������, ���������� �� ���� ��� ���������� ������� � ������� ������
private:
    QList<QWebSocket*> clientsSocks; 
    void stopWaitingLoop(); //�������� ������� ��� ��������� �������
    std::thread *m_thread;
public slots: 
    void clientConnected(); //���� ��� ������ ������� � ����������
    void messageReceived(const QString &msg);
    void clientDisconnected(); //���� ��� ������ ������� �� ���������� �������
};

