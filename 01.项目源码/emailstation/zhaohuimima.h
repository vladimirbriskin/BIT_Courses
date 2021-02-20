#ifndef ZHAOHUIMIMA_H
#define ZHAOHUIMIMA_H

#include <QWidget>
#include<QTcpSocket>
namespace Ui {
class zhaohuimima;
}

class zhaohuimima : public QWidget
{
    Q_OBJECT

public:
    explicit zhaohuimima(QWidget *parent = nullptr);
    ~zhaohuimima();
    void mysl();

private:
    Ui::zhaohuimima *ui;
    QTcpSocket socket;
signals:
    void  mysls();
private slots:
    void on_pushButton_clicked();
};

#endif // ZHAOHUIMIMA_H
