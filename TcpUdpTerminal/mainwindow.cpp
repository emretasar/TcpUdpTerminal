#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pTcpServer(new TcpServer(this))
    , m_pTcpClient(new TcpClient(this))
{
    ui->setupUi(this);
    m_pTcpServer->setActionBrowser(*ui->textBrowserActionsTcpServer);
    m_pTcpClient->setActionBrowser(*ui->textBrowserActionsTcpClient);

    createConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createConnections()
{
    connect(ui->pushButtonConnectionTcpServer, &QPushButton::clicked, this, &MainWindow::onTcpServerListenButtonClicked);
    connect(ui->pushButtonConnectionTcpClient, &QPushButton::clicked, this, &MainWindow::onTcpClientConnectButtonClicked);
    connect(ui->pushButtonSendTcpClient, &QPushButton::clicked, this, &MainWindow::onTcpClientSendButtonClicked);
    connect(ui->pushButtonSendTcpServer, &QPushButton::clicked, this, &MainWindow::onTcpServerSendButtonClicked);

    connect(&(*m_pTcpServer), &TcpServer::messageReceived, this, &MainWindow::onTcpServerReceivedMessage);
    connect(&(*m_pTcpClient), &TcpClient::messageReceived, this, &MainWindow::onTcpClientReceivedMessage);
}

void MainWindow::onTcpServerListenButtonClicked()
{
    bool isPortValid = false;
    quint16 port = ui->lineEditPortTcpServer->text().toUInt(&isPortValid, 10);
    if (!isPortValid) {
        qDebug() << "Listening port number is not valid for server";
        ui->textBrowserActionsTcpServer->append("Listening port number is not valid for server");
        return;
    }
    m_pTcpServer->startServer(port);
}

void MainWindow::onTcpClientConnectButtonClicked()
{
    bool isPortValid = false;
    QHostAddress hostAddress;
    quint16 port = ui->lineEditPortTcpClient->text().toUInt(&isPortValid, 10);
    QString ip = ui->lineEditIpTcpClient->text();
    if (!isPortValid || !hostAddress.setAddress(ip)) {
        qDebug() << "Connection port number or host IP is not valid for client";
        ui->textBrowserActionsTcpClient->append("Connection port number or host IP is not valid for client");
        return;
    }
    m_pTcpClient->connectToServer(ip, port);
}

void MainWindow::onTcpClientSendButtonClicked()
{
    QString message = ui->plainTextEditSendTcpClient->toPlainText();
    m_pTcpClient->sendMessage(message);
}

void MainWindow::onTcpServerSendButtonClicked()
{
    QString message = ui->plainTextEditSendTcpServer->toPlainText();
    m_pTcpServer->sendMessage(message);
}

void MainWindow::onTcpServerReceivedMessage(const std::string& message)
{
    qDebug() << "onTcpServerReceivedMessage" << message;
    m_strTcpServerReceivedMessage += (message + " ");
    ui->textBrowserRecDataTcpServer->setText(QString::fromStdString(m_strTcpServerReceivedMessage));
    ui->textBrowserSentDataTcpClient->setText(QString::fromStdString(m_strTcpServerReceivedMessage));
}

void MainWindow::onTcpClientReceivedMessage(const std::string& message)
{
    qDebug() << "onTcpServerReceivedMessage" << message;
    m_strTcpClientReceivedMessage += (message + " ");
    ui->textBrowserRecDataTcpClient->setText(QString::fromStdString(m_strTcpClientReceivedMessage));
    ui->textBrowserSentDataTcpServer->setText(QString::fromStdString(m_strTcpClientReceivedMessage));
}
