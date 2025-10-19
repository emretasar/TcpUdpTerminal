#include "mainwindow.h"
#include "tcpserver.h"
#include "tcpclient.h"
#include <QTimer>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;
    // w.show();
    TcpServer server;
    server.startServer(12345);

    TcpClient client;

    QTimer::singleShot(1000, [&client]() {
        client.connectToServer("127.0.0.1", 12345);
    });

    QTimer::singleShot(2000, [&client]() {
        client.sendMessage("Hello from client!");
    });
    return a.exec();
}
