#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QComboBox>
#include<QDebug>
#include<QString>
#include<QTcpSocket>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);

    tcpSocket=NULL;
    tcpSocket = new QTcpSocket(this);
    //暂时调试

     //start.show();
    ui->widget_2->setStyleSheet("background-image: url(:/new/prefix1/img/1.jpg)");
   //切换主题信号
    connect(ui->comboBox_2,&QComboBox::currentTextChanged,
       [=]()
       {
      int i= ui->comboBox_2->currentIndex();
      if(i==0)
      {
        ui->widget_2->setStyleSheet("background-image: url(:/new/prefix1/img/1.jpg)");

      }
      else if(i==1)
        {

           ui->widget_2->setStyleSheet("background-image: url(:/new/prefix1/img/heise.jpg)");

      }
      else if(i==2)
      {
          ui->widget_2->setStyleSheet("background-image: url(:/new/prefix1/img/huangse.jpg)");
      }
      else
      {
           ui->widget_2->setStyleSheet("background-image: url(:/new/prefix1/img/hongse.jpg)");
      }

       });
      //成功连接服务器
       connect(tcpSocket,&QTcpSocket::connected,
               [=]()
               {
                qDebug()<< "kkk";
               }

               );



    //切换字体颜色触发函数
    connect(ui->comboBox,&QComboBox::currentTextChanged,
            [=]()
            {
              int i= ui->comboBox->currentIndex();
                if(i==0){


                }

            }
                );

    //回到登录界面
    connect(&start,&kehuduan::mysignal,
            [=]()
           {
             this->show();
             start.hide();
             tcpSocket->abort();
           }
            );
    //回到登录端
   connect(&huce,&zhuce::mysignal,
           [=]()
       {
         this->show();
         huce.close();
       }
          );
    //回到登录端
   connect(&zhaohu,&zhaohuimima::mysls,
           [=]()
       {
         this->show();
         zhaohu.close();
       }

           );
    //关闭登录界面
    connect(ui->pushButton_2,&QPushButton::clicked,
            [=]()
           {
            this->close();
             tcpSocket->close();
          }



            );
    //注册信号
    connect(ui->pushButton_3,&QPushButton::clicked,
            [=](){
             huce.show();
             this->close();
            }


            );
    //找回密码
    connect(ui->pushButton_4,&QPushButton::clicked,
            [=](){
              this->hide();
              zhaohu.show();
            }


            );

}

MainWindow::~MainWindow()
{
    delete ui;
    tcpSocket->close();
}


void MainWindow::on_pushButton_clicked()//链接客户端
{

      QString id=this->ui->lineEdit_2->displayText();
      QString password=ui->lineEdit_3->displayText();
      int len1=id.length();
      int len2=password.length();
      if(len1<6||len1>16)
      {

          this->ui->label_8->setText("用户名错误");
      }
      if(len1>=6&&len1<=16)
      {

          this->ui->label_8->setText("用户名正确");
      }
      if(len2<6||len2>16)
      {
           this->ui->label_9->setText("密码错误");
      }
      if(len2>=6&&len2<=16)
      {

          this->ui->label_9->setText("密码正确");
      }
      if(len1<=16&&len1>=6&&len2<=16&&len2>=6)
      {

          //登录跳转
          QString ip  = "172.20.10.12" ;
          qint16  port= 5000 ;
          tcpSocket->abort();
          tcpSocket->connectToHost(ip,port);
          tcpSocket->write("00");//登录专用数字00
          tcpSocket->write(id.toUtf8());
          tcpSocket->write("#");//登录分隔符
          //tcpSocket->write(password.toUtf8());
          qint64 il =tcpSocket->write(password.toUtf8());
          if(il>0)
          {
              qDebug()<<"xiu";
          }
          connect(tcpSocket,&QTcpSocket::readyRead,
                  [=]()
                  {

                   QByteArray isready=tcpSocket->readAll();
                    int ttt=isready.toInt();
                    qDebug()<<ttt;

                   //开始判断服务器是否连接成功


                     if(ttt==1)
                     {
                    tcpSocket->disconnectFromHost();
                    this->hide();
                    start.show();


                     }
                    else{

                         //QMessageBox::warning(NULL, "Title", "搞错了，重填", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                             tcpSocket->disconnectFromHost();
                            }


                      }

                  );
           //this->hide();
           //start.show();





      }

}
