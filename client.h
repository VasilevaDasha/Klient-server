#pragma once
#include <QWebSocket>
#include <QObject>
#include <QThread>
#include <QTextStream>

class Client: public QObject
{
    Q_OBJECT //����� ����� ������������ ������� � �����

public:
    explicit Client(QObject *parent = nullptr);
    ~Client(); //����������, � ������� ���������� ������� ������ �� ����, ���������� ��� ���������� ����� � ������� ������
signals:
    void needToSendMessage(const QString msg); //������ ������������ �������� �� �������� ���������
public slots:
    void serverMessageReceived(const QByteArray &bytes); //���� ��� ������ ������� � ���������
    void messageReceived(const QString &msg);
private:
    void messageWaitingLoop(); //������� ��������� ��� ��������� �������
    void connectToServer(const QPair<QString,QString> connectionData); 
    QWebSocket *socket;
    std::thread *m_thread;
};
