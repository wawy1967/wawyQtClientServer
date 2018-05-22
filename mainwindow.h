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
    void initializeChat();
private slots:
    void on_pushButtonSend_clicked();
    void incomingMessage();

private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QTcpSocket *socket;
};

#endif // MAINWINDOW_H
