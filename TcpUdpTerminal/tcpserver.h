#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class TcpServer : public QObject
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *parent = nullptr);
    bool startServer(quint16 port);
    void sendMessage(const QString &message);

private slots:
    void onNewConnection();
    void onReadyRead();

signals:
    void messageReceived(std::string message);

private:
    QTcpServer *server;
    QList<QTcpSocket*> clients;
};

#endif // TCPSERVER_H
