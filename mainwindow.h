#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initializeServer();
    void initializeClient();
private slots:
    void connected();
    void disconnected();
    void socketRead();
    void socketWrite(qint64);
    void on_pushButtonConnect_clicked();
    void on_pushButtonSend_clicked();
    void newServerConnection();

private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QTcpSocket *client;
    QTcpSocket *socket;
};

#endif // MAINWINDOW_H
