#include "zhaohuimima.h"
#include "ui_zhaohuimima.h"
#include<QTcpSocket>
#include<QMessageBox>
zhaohuimima::zhaohuimima(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::zhaohuimima)
{
    ui->setupUi(this);
    //socket=NULL;
    connect(ui->pushButton_2,&QPushButton::clicked,this,&zhaohuimima::mysl);
    connect(&socket,&QTcpSocket::connected,
            [=]()
            {
        QString a1=ui->lineEdit->displayText();
        QString a2=ui->lineEdit_2->displayText();
        socket.write("02");
        socket.write(a1.toUtf8());
        socket.write("#");
        socket.write(a2.toUtf8());


            }

            );
    connect(&socket,&QTcpSocket::readyRead,
            [=]()
           {
            QByteArray data=socket.readAll();
            QString sting=QString(data);
            QMessageBox::information(NULL, "新密码", sting, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            socket.disconnectFromHost();
           }
           );
}

void zhaohuimima::mysl()
{
    emit mysls();


}
zhaohuimima::~zhaohuimima()
{
    delete ui;
}

void zhaohuimima::on_pushButton_clicked()
{
    QString ip  = "172.20.10.12" ;
    qint16  port= 5000 ;
    socket.abort();
    socket.connectToHost(ip,port);
}
