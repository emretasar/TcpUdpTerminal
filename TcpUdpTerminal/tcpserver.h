#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QTextBrowser>

class TcpServer : public QObject
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *parent = nullptr);
    bool startServer(quint16 port);
    void sendMessage(const QString &message);
    void setActionBrowser(QTextBrowser& actionBrowser);

private slots:
    void onNewConnection();
    void onReadyRead();

signals:
    void messageReceived(std::string message);

private:
    QTcpServer *server;
    QList<QTcpSocket*> clients;
    QTextBrowser *m_tbActionTextBrowser;
};

#endif // TCPSERVER_H
