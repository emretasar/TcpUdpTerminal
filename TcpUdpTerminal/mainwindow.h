#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpserver.h"
#include "tcpclient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTcpServerListenButtonClicked();
    void onTcpClientConnectButtonClicked();
    void onTcpClientSendButtonClicked();
    void onTcpServerSendButtonClicked();
    void onTcpServerReceivedMessage(const std::string& message);
    void onTcpClientReceivedMessage(const std::string& message);

private:
    Ui::MainWindow *ui;
    TcpServer *m_pTcpServer;
    TcpClient *m_pTcpClient;

    std::string m_strTcpServerReceivedMessage = "";
    std::string m_strTcpClientReceivedMessage = "";

    void createConnections();
};
#endif // MAINWINDOW_H
