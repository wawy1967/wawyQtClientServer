#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    client = new QTcpSocket(this);
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
    if (!server->listen(QHostAddress::Any,1234))
    {
        ui->textBrowser->append("Cannot connect to server!");
    }
    else
    {
        ui->textBrowser->append("Server started!");
    }

    client = new QTcpSocket(this);
    connect(client,SIGNAL(connected()),this,SLOT(connected()));
    connect(client,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(client,SIGNAL(readyRead()),this,SLOT(socketRead()));
    connect(client,SIGNAL(bytesWritten(qint64)),this,SLOT(socketWrite(qint64)));
}

MainWindow::~MainWindow()
{
    client->close();
    delete ui;
}

void MainWindow::on_pushButtonConnect_clicked()
{
    ui->textBrowser->append("Connecting...");
    client->connectToHost(ui->lineEdit->text(),1234);
    if (!client->waitForConnected(2000))
    {
        ui->textBrowser->append("Fail to connect.");
        ui->textBrowser->append(client->errorString());
    }
    else
    {
        ui->textBrowser->append("Connected at:" + ui->lineEdit->text());
    }
}

void MainWindow::connected()
{
    ui->textBrowser->append("Connected");
    client->write("Client: Hello Server");
}
void MainWindow::disconnected()
{
    ui->textBrowser->append("Disconnected");
}
void MainWindow::socketRead()
{
    ui->textBrowser->append("Reading...");
    ui->textBrowser->append(client->readAll());
}
void MainWindow::socketWrite(qint64 bytes)
{
    ui->textBrowser->append("We write "+QString::number(bytes));
}

void MainWindow::newConnection()
{
    client = server->nextPendingConnection();
    client->waitForReadyRead(3000);
    ui->textBrowser->append(client->readAll());
    client->write("Server: Hello client!");
    client->flush();
    client->waitForBytesWritten(3000);
    client->close();
}

void MainWindow::on_pushButtonSend_clicked()
{
    QString msg = ui->lineEditSend->text();
    client->write("Hello");
}
