#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeServer();
    initializeClient();
}
//******************************************************************************
void MainWindow::initializeServer()
{
    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any,1234))
        ui->textBrowser->append("Cannot connect to server!");
    else
        ui->textBrowser->append("Server started!");
    connect(server,SIGNAL(newConnection()),this,SLOT(newServerConnection()));
}
void MainWindow::newServerConnection()
{
    socket = new QTcpSocket(this);
    socket = server->nextPendingConnection();
    socket->waitForReadyRead(3000);
    ui->textBrowser->append(socket->readAll());
    socket->write("Windows: Hello");
    socket->flush();
    socket->waitForBytesWritten(3000);
    socket->close();
}
//********************************************************************************
void MainWindow::initializeClient()
{
    client = new QTcpSocket(this);
    connect(client,SIGNAL(connected()),this,SLOT(connected()));
    connect(client,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(client,SIGNAL(readyRead()),this,SLOT(socketRead()));
    connect(client,SIGNAL(bytesWritten(qint64)),this,SLOT(socketWrite(qint64)));
}
void MainWindow::on_pushButtonConnect_clicked()
{
    ui->textBrowser->append("Connecting...");
    client->connectToHost(ui->lineEdit->text(),1234);
    if (!client->waitForConnected(2000))
        ui->textBrowser->append("Fail to connect.");
    else
        ui->textBrowser->append("Connected at:" + ui->lineEdit->text());
}
void MainWindow::connected()
{
    ui->textBrowser->append("Connected");    
}
void MainWindow::disconnected()
{
    ui->textBrowser->append("Disconnected");
}
void MainWindow::socketRead() //client
{
    ui->textBrowser->append(client->readAll());
}
void MainWindow::socketWrite(qint64 bytes) //client
{
    client->write("Windows: Hello");
    ui->textBrowser->append("We write " + QString::number(bytes));
}
void MainWindow::on_pushButtonSend_clicked() //client
{
    QString msg = ui->lineEditSend->text();
    client->write(msg.toUtf8());
}
//********************************************************************************
MainWindow::~MainWindow()
{
    client->close();
    server->close();
    delete ui;
}
