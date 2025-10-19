#include "TcpClient.h"
#include <QDebug>

TcpClient::TcpClient(QObject *parent)
    : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
}

void TcpClient::connectToServer(const QString &host, quint16 port)
{
    qDebug() << "Connecting to server...";
    socket->connectToHost(host, port);
}

void TcpClient::sendMessage(const QString &message)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(message.toUtf8());
        socket->flush();
    } else {
        qDebug() << "Not connected to server!";
    }
}

void TcpClient::onConnected()
{
    qDebug() << "Connected to server!";
}

void TcpClient::onReadyRead()
{
    QByteArray data = socket->readAll();
    qDebug() << "Client received:" << data;
}

void TcpClient::onDisconnected()
{
    qDebug() << "Disconnected from server.";
}
