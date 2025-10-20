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
    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "Server could not start:" << server->errorString();
        return false;
    }

    qDebug() << "Server started on port" << port;
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
}

void TcpServer::onNewConnection()
{
    while (server->hasPendingConnections()) {
        QTcpSocket *client = server->nextPendingConnection();
        clients << client;

        qDebug() << "New client connected from" << client->peerAddress().toString();

        connect(client, &QTcpSocket::readyRead, this, &TcpServer::onReadyRead);
        connect(client, &QTcpSocket::disconnected, this, [this, client]() {
            qDebug() << "Client disconnected.";
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
    emit messageReceived(data.toStdString());
    // client->write("Server echo: " + data);
}
