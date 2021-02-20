#include "zhuce.h"
#include "ui_zhuce.h"
#include<outsides.h>
#include<QDialog>
#include<QDebug>
#include<QMessageBox>
#include<QTcpSocket>
zhuce::zhuce(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::zhuce)
{
    panduan1=0;
    panduan2=0;
    panduan3=0;
    panduan4=0;
    ui->setupUi(this);
    tcpSocket=NULL;
    tcpSocket =new QTcpSocket(this);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&zhuce::mysl);
    connect(ui->pushButton,&QPushButton::clicked,
         [=]()
    {
        if(panduan1==1&&panduan2==1&&panduan3==1&&panduan4==1)
        {
            qDebug()<<"hhh";
            QString ip  = "172.20.10.12" ;
            qint16  port= 5000 ;
            tcpSocket->abort();
            tcpSocket->connectToHost(ip,port);
            connect(tcpSocket,&QTcpSocket::connected,
                    [=]()
                    {
                QString a1=ui->lineEdit->displayText();
                QString a2=ui->lineEdit_2->displayText();
                QString a3=ui->lineEdit_3->displayText();
                QString a4=ui->lineEdit_4->displayText();
                tcpSocket->write("01");
                tcpSocket->write(a1.toUtf8());
                tcpSocket->write("#");
                tcpSocket->write(a2.toUtf8());
                tcpSocket->write("#");
                tcpSocket->write(a3.toUtf8());
                tcpSocket->write("#");
                tcpSocket->write(a4.toUtf8());


                    }

                    );
            connect(tcpSocket,&QTcpSocket::readyRead,
                    [=]()
                    {
                        QByteArray bet=tcpSocket->readAll();
                        int ttt=bet.toInt();
                        qDebug()<<ttt;
                        if(ttt==1)
                        {
                        QMessageBox::information(NULL, "好的骚年", "请回到登录端", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                        tcpSocket->disconnectFromHost();
                        }
                        else
                        {
                            QMessageBox::information(NULL, "不行", "用户名被别人搞过了换一个", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                            tcpSocket->disconnectFromHost();

                        }
                    }
                    );

        }
        else
         {
           QMessageBox::information(NULL, "Title", "你这不行，重填！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

          }

    }

            );


}

zhuce::~zhuce()
{
    delete ui;
}
void zhuce::mysl()
{

    emit mysignal();

}
void zhuce::on_lineEdit_textChanged(const QString &arg1)
{
    QByteArray ba=arg1.toLatin1();
        char *c=ba.data();
        int len=strlen(c);
        int i;
        int flag=0;

        if((c[0]>='a'&&c[0]<='z')||(c[0]>='A'&&c[0]<='Z'))
        {
            if(len<6||len>16)
            {
                this->ui->label_5->setText("你这整的不行，得是6~16位");
                panduan1=0;
            }

            else
            {
                flag=0;

                for(i=0;i<len;i++)
                {
                    if((c[i]>='0'&&c[i]<='9')||(c[i]>='a'&&c[i]<='z')||(c[i]>='A'&&c[i]<='Z'))
                    {

                    }
                    else
                    {
                       this->ui->label_5->setText("咋还给我整出非法字符了呢，只能是字母和数字");
                        flag++;panduan1=0;
                    }

                 }
                if(flag==0){ this->ui->label_5->setText("成了,写下一行吧");
                panduan1=1;}

            }
        }

        else
        {
            this->ui->label_5->setText("您瞅清楚了，得是字母打头");panduan1=0;
        }

}

void zhuce::on_lineEdit_2_textChanged(const QString &arg1)
{
    QByteArray ba=arg1.toLatin1();
       char *c2=ba.data();
       int len=strlen(c2);

           if(len<6||len>16)
           {
               this->ui->label_6->setText("你这整得不行，得是6~16位");panduan2=0;
           }

          else
           {this->ui->label_6->setText("成了，写下一行吧");
           originpass=c2;panduan2=1;
           }
}

void zhuce::on_lineEdit_3_textChanged(const QString &arg1)
{
    std::string c3 = arg1.toStdString();
        if(c3==originpass)
        {
            this->ui->label_7->setText("成了，写下一行吧");panduan3=1;

        }

        else
        {
            this->ui->label_7->setText("你这两个密码不一样啊");panduan3=0;
        }
}

void zhuce::on_lineEdit_4_textChanged(const QString &arg1)
{
    QByteArray ba=arg1.toLatin1();
        char *c4=ba.data();
        int len=strlen(c4);

            if(len!=11)
            {
                this->ui->label_8->setText("你这整得是电话号码？");
                panduan4=0;
            }

           else
            {
                this->ui->label_8->setText("成了， 提交试试吧");panduan4=1;
            }
}
