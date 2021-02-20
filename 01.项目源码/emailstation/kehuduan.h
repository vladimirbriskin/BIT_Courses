#ifndef KEHUDUAN_H
#define KEHUDUAN_H

#include <QWidget>
#include<QPushButton>
#include<QFile>
#include<QTcpSocket>
#include<QTimer>
#include<QSqlTableModel>
#include<QListWidgetItem>
#include<QTime>
namespace Ui {
class kehuduan;
}

class kehuduan : public QWidget
{
    Q_OBJECT

public:
    explicit kehuduan(QWidget *parent = nullptr);
    ~kehuduan();
    void  senddata();
    void sensinal();
    void  receivefile();
    void  Sleep(int );

private:
    Ui::kehuduan *ui;
    QFile file1;
    QFile file2;
    QString  fileName;
    qint64  fileSize;
    QString  fileName2;
    qint64 fileSize2;
    QTcpSocket *tcp;
    qint64  sendsize;
    qint64  receivesize;
    QTimer timer;
    QSqlTableModel *model;
    bool  isstart1;
    bool  isstart2;
    int check;

signals:
    void mysignal();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_listWidget_2_itemClicked(QListWidgetItem *item);
    void on_listWidget_3_itemClicked(QListWidgetItem *item);
    void on_pushButton_12_clicked();
    void on_pushButton_4_clicked();

};

#endif // KEHUDUAN_H
