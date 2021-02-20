#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"kehuduan.h"
#include"zhuce.h"
#include"zhaohuimima.h"
#include<QTcpSocket>
#include<QAbstractSocket>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    kehuduan start;
    QTcpSocket *tcpSocket;
    zhuce huce;
    zhaohuimima zhaohu;
};
#endif // MAINWINDOW_H
