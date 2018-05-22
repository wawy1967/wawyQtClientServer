#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeChat();
}
//******************************************************************************
void MainWindow::initializeChat()
{
    ui->statusBar->showMessage("No connection!");
    socket = new QTcpSocket(this);
    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any,1234))
        ui->textBrowser->append("Cannot listen!");
    else
        ui->textBrowser->append("Listening...");
    connect(server,SIGNAL(newConnection()),this,SLOT(incomingMessage()));
    connect(ui->lineEditSend,SIGNAL(returnPressed()),
            ui->pushButtonSend,SLOT(click()));
}
void MainWindow::incomingMessage()
{   
    socket = server->nextPendingConnection();
    socket->waitForReadyRead(3000);
    ui->textBrowser->setTextColor(Qt::blue);
    ui->textBrowser->append("Dia: " + socket->readAll());
    socket->close();
}
//********************************************************************************

void MainWindow::on_pushButtonSend_clicked()
{
    socket->connectToHost(ui->lineEdit->text(),1234);
    if (!socket->waitForConnected(1000))
    {
        ui->textBrowser->append("Fail to connect.");
        ui->statusBar->showMessage("No connection!");
    }
    else
    {
        ui->statusBar->showMessage("Connected at:" +
                                   socket->peerAddress().toString().toUtf8());
        socket->write(ui->lineEditSend->text().toUtf8());
        socket->flush();
        socket->waitForBytesWritten(1000);
        ui->textBrowser->setTextColor(Qt::red);
        ui->textBrowser->append("Aku: " + ui->lineEditSend->text().toUtf8());
    }
    socket->close();
}

//********************************************************************************
MainWindow::~MainWindow()
{
    server->close();
    socket->close();
    delete ui;
}
