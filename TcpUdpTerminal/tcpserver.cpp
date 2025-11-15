#include "TcpServer.h"
#include <QDebug>

TcpServer::TcpServer(QObject *parent)
    : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &TcpServer::onNewConnection);
}

bool TcpServer::startServer(quint16 port)
{
    qDebug() << "TcpServer::startServer" << port;
    m_tbActionTextBrowser->append("TcpServer::startServer: " + QString::number(port));
    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "Server could not start: " << server->errorString();
        m_tbActionTextBrowser->append("Server could not start: " + server->errorString());
        return false;
    }

    qDebug() << "Server started on port" << port;
    m_tbActionTextBrowser->append("Server started on port: " + QString::number(port));
    return true;
}

void TcpServer::sendMessage(const QString &message)
{
    QByteArray data = message.toUtf8();

    for (QTcpSocket *client : clients) {
        if (client->state() == QAbstractSocket::ConnectedState) {
            client->write(data);
            client->flush();
        }
    }

    qDebug() << "Server sent:" << message;
    m_tbActionTextBrowser->append("Server sent: " + message);
}

void TcpServer::setActionBrowser(QTextBrowser &actionBrowser)
{
    m_tbActionTextBrowser = &actionBrowser;
}

void TcpServer::onNewConnection()
{
    while (server->hasPendingConnections()) {
        QTcpSocket *client = server->nextPendingConnection();
        clients << client;

        qDebug() << "New client connected from" << client->peerAddress().toString();
        m_tbActionTextBrowser->append("New client connected from: " + client->peerAddress().toString());

        connect(client, &QTcpSocket::readyRead, this, &TcpServer::onReadyRead);
        connect(client, &QTcpSocket::disconnected, this, [this, client]() {
            qDebug() << "Client disconnected.";
            m_tbActionTextBrowser->append("Client disconnected.");
            clients.removeOne(client);
            client->deleteLater();
        });
    }
}

void TcpServer::onReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client)
        return;

    QByteArray data = client->readAll();
    qDebug() << "Server received:" << data;
    m_tbActionTextBrowser->append("Server received: " + data);
    emit messageReceived(data.toStdString());
    // client->write("Server echo: " + data);
}
