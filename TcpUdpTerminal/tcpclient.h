#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QObject>

class TcpClient : public QObject
{
    Q_OBJECT

public:
    explicit TcpClient(QObject *parent = nullptr);
    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QString &message);

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();

signals:
    void messageReceived(const std::string& message);

private:
    QTcpSocket *socket;
};

#endif // TCPCLIENT_H
