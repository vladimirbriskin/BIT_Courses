#ifndef ZHUCE_H
#define ZHUCE_H

#include <QWidget>
#include<QTcpSocket>

namespace Ui {
class zhuce;
}

class zhuce : public QWidget
{
    Q_OBJECT

public:
    explicit zhuce(QWidget *parent = nullptr);
    ~zhuce();
    void mysl();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_lineEdit_4_textChanged(const QString &arg1);

private:
    Ui::zhuce *ui;
    QTcpSocket *tcpSocket;
signals:
    void  mysignal();
};

#endif // ZHUCE_H
