#include "kehuduan.h"
#include "ui_kehuduan.h"
#include<QPushButton>
#include<QDebug>
#include<QFileDialog>
#include<QFileInfo>
#include<QDebug>
#include<QMessageBox>
#include<outsides.h>
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include<QVariantList>
#include<QSqlTableModel>
#include<QSqlRecord>
#include<QTcpSocket>
#include<QVariantList>
#include<QSqlError>
#include<QListWidgetItem>
#include<QListWidget>
#include<QDataStream>
kehuduan::kehuduan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::kehuduan)
{
    ui->setupUi(this);
    tcp=NULL;
    tcp= new QTcpSocket(this);
    mygod="kk";
    check=0;
     isstart1=true;
     //ui->pushButton_2
    connect(tcp,&QTcpSocket::connected,
            [=](){

            qDebug()<<"连接好了";
            //发送附件
            if(mygod!="kk"){
                //qDebug()<<mygod;
                tcp->write("05");

            qint64 iq=tcp->write(mygod.toUtf8().data());
            if(iq>0)
          {
              tcp->write("#");
              Sleep(50);
                senddata();
          }


           }
    }
            );
     //加标题
    ui->listWidget_2->addItem("发件人id                       信件标题                         内容");
     connect(ui->pushButton,&QPushButton::clicked,this,& kehuduan::sensinal );
      ui->page_5->show();
      ui->page_6->hide();
      ui->page_2->hide();
      ui->page_7->hide();
      connect(ui->pushButton_3,&QPushButton::clicked,
           [=](){

          ui->page_5->show();
          ui->page_6->hide();
          ui->page_2->hide();
          ui->page_7->hide();
          ui->page_8->hide();
            }


           );
      connect(ui->pushButton_9,&QPushButton::clicked,
           [=](){

            ui->page_5->show();

            ui->page_6->hide();
            ui->page_2->hide();
            ui->page_7->hide();
            ui->page_8->hide();

            }
        );
      ui->widget_3->setStyleSheet("background-image: url(:/new/prefix1/img/1.jpg)");
     //切换主题信号
      connect(ui->comboBox_2,&QComboBox::currentTextChanged,
         [=]()
         {
        int i= ui->comboBox_2->currentIndex();
        if(i==0)
        {
          ui->widget_3->setStyleSheet("background-image: url(:/new/prefix1/img/1.jpg)");

        }
        else if(i==1)
          {

             ui->widget_3->setStyleSheet("background-image: url(:/new/prefix1/img/heise.jpg)");

        }
        else if(i==2)
        {
            ui->widget_3->setStyleSheet("background-image: url(:/new/prefix1/img/huangse.jpg)");
        }
        else
        {
             ui->widget_3->setStyleSheet("background-image: url(:/new/prefix1/img/hongse.jpg)");
        }

         });
      //切换字体颜色
      connect(ui->comboBox_3,&QComboBox::currentTextChanged,
         [=]()
         {
        int i= ui->comboBox_3->currentIndex();
        if(i==0)
        {
          ui->widget_3->setStyleSheet("color:#2b2b2b");

        }
        else if(i==1)
          {

             ui->widget_3->setStyleSheet("color:#00aaff");

        }
        else if(i==2)
        {
            ui->widget_3->setStyleSheet("color:#00ff7f");
        }
        else
        {
             ui->widget_3->setStyleSheet("color:#00ffff");
        }

         });
      //链接数据库


}

void kehuduan::sensinal()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->textEdit_2->clear();
    tcp->disconnectFromHost();
    emit mysignal();


}

kehuduan::~kehuduan()
{
    delete ui;
}


void kehuduan::on_pushButton_2_clicked()
{
    this->close();
}
//选择文件按钮触发
void kehuduan::on_pushButton_15_clicked()
{
    QString filePath=QFileDialog::getOpenFileName(this,"open","../");
    if(false==filePath.isEmpty())
    {

        fileName.clear();
        fileSize=0;
        QFileInfo  info(filePath);
        fileName=info.fileName();
        fileSize=info.size();
        file1.setFileName(filePath);
        file1.open(QIODevice::ReadOnly);
        this->ui->textEdit->setText(filePath);
        QString filesize=QString::number(fileSize);
        mygod = QString("%1#%2").arg(fileName).arg(fileSize);
    }
    else
    {
        qDebug()<<"文件路径出错";

   }
}
//发送文件按钮
void kehuduan::on_pushButton_5_clicked()
{
    QString ip  = "172.20.10.12" ;
    qint16  port= 5000 ;
    tcp->abort();
    tcp->connectToHost(ip,port);//这个后面进行改动

    QString p1=this->ui->lineEdit->displayText();
    QString p2=this->ui->lineEdit_2->displayText();
    if(this->ui->textEdit_2->toPlainText()!=""&&p1!=""&&p2!="")
     {
    tcp->write("03");
    tcp->write(p1.toUtf8().data());//id
    tcp->write("#");
    tcp->write(p2.toUtf8().data());//主题
    QString sting=this->ui->textEdit_2->toPlainText();
     tcp->write("#");
    qint64 it=tcp->write(sting.toUtf8().data());
   if(it>0)
   {
     QMessageBox::information(NULL, "成功", "发送成功", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
   }
   else
 {
     QMessageBox::information(NULL, "失败", "发送失败，重新发送", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
     file1.close();
     tcp->disconnectFromHost();
 }

  }
}
void kehuduan::senddata()
{
   sendsize=0;
   qint64 len=0;
   do{
        char buf[4*1024]={0};
        len=0;
        len=file1.read(buf,sizeof(buf));
        len=tcp->write(buf,len);
        sendsize+=len;
        //qDebug()<<buf<<sendsize<<fileSize<<len;
    }
   while(len>0);

   if(sendsize==fileSize)
     {

        file1.close();
        tcp->disconnectFromHost();
           QMessageBox::information(NULL, "成功", "附件发送成成功");
     }
   else{
    QMessageBox::information(NULL, "失败", "附件发送失败，重新发送吧", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    tcp->disconnectFromHost();
   }
}
//点击存进草稿箱
void kehuduan::on_pushButton_6_clicked()
{
    QString sting= this->ui->textEdit_2->toPlainText();

    this->ui->listWidget->addItem("今日份草稿:          "+sting);



}
//收信按钮
void kehuduan::on_pushButton_10_clicked()
{

    mygod="kk";isstart1=true;
    ui->textEdit->clear();
    ui->textEdit_3->clear();
    QString ip  = "172.20.10.12" ;
    qint16  port= 5000 ;
    tcp->abort();
    tcp->connectToHost(ip,port);
    //收信信号
       connect(tcp,&QTcpSocket::connected,
            [=]()
             {
          if(isstart1==true)
            tcp->write("04");

            }

            );
      connect(ui->pushButton_18,&QPushButton::clicked,
              [=]()mutable
      {
          tcp->disconnectFromHost();
          QString ip  = "172.20.10.12" ;
          qint16  port= 5000 ;
          tcp->abort();
          tcp->connectToHost(ip,port);
          isstart1=false;
          isstart2=true;
          tcp->write("06");

      }
              );
       connect(tcp,&QTcpSocket::readyRead,
               [=]()mutable
     {

                 QByteArray pp=tcp->readAll();
                 QString qq=QString(pp);
                 ui->textEdit_3->setText(qq);
                 QString st=ui->textEdit_3->toPlainText();
                 qDebug()<<qq;
                 if(st!=""&&isstart1==true)
                 {

                     int i;
                     QString s=st;
                     QString a1="";
                     QString a2="";
                     QString a3="";
                     for(i=0;i<s.length();i++)
                     {
                        if(s[i]!='#')
                         {
                            a1.append(s[i]);

                         }
                        else break;


                     }
                     i++;
                     for(;i<s.length();i++)
                     {
                        if(s[i]!='#')
                         {
                            a2.append(s[i]);

                         }
                        else break;


                     }
                     i++;
                     for(;i<s.length();i++)
                     {
                        if(s[i]!='#')
                         {
                            a3.append(s[i]);
                         }

                        else
                         {
                            break;
                         }

                     }

                     this->ui->listWidget_2->addItem(a1+"                   "+a2+"                     "+a3);


                 }

               else if(isstart1==false)
               {
                 if(isstart2==true)

                {
                   receivesize=0;

                   fileName2=qq.section("#",0,0);
                   fileSize2=qq.section("#",1,1).toInt();
                    qDebug()<<qq<<fileName2<<fileSize2;
                  file2.setFileName(fileName);
                  bool isOk=file2.open(QIODevice::WriteOnly);
                  if(isOk==false)
                  {

                      QMessageBox::warning(this,"文件打开错误","错误");
                  }
                    isstart2=false;
                 }

                        else
                       {
                            qDebug()<<pp;
                            qint64 len= file2.write(pp);
                             receivesize+=len;
                             qDebug()<<len;
                            if(receivesize==fileSize2)
                            {
                                file2.close();
                                 QMessageBox::information(this,"完成","文件接收完成");
                                 ui->textEdit_4->setText("接收到一个文件，请自行查看");
                                 tcp->disconnectFromHost();
                            }

                        }
               }


                 ui->textEdit_3->clear();


              }
               );
      this->ui->page_5->hide();
      this->ui->page_7->hide();
      this->ui->page_2->hide();
       this->ui->page_8->hide();
      this->ui->page_6->show();

}


//草稿箱跳转
void kehuduan::on_pushButton_11_clicked()
{
    ui->textEdit->clear();
     this->ui->page_7->show();
     this->ui->page_5->hide();
     this->ui->page_6->hide();
    this->ui->page_2->hide();
    this->ui->page_8->hide();
}


//草稿箱操作
void kehuduan::on_listWidget_itemClicked(QListWidgetItem *item)
{
 connect(ui->pushButton_13,&QPushButton::clicked,
         [=]()
         {
     QString sting=item->text();
     qDebug()<<sting;
     QString io;
     int j=0;
     for(int i=16;i<sting.length();i++)
     {
         io[j++]=sting[i];

     }
     this->ui->textEdit_2->setText(io);
     this->ui->page_5->show();
     this->ui->page_7->hide();
     this->ui->page_6->hide();
     this->ui->page_2->hide();
     this->ui->page_8->hide();
         }

         );
    connect(ui->pushButton_14,&QPushButton::clicked,
         [=]()
         {
         foreach(QModelIndex index,ui->listWidget->selectionModel()->selectedIndexes()){


                    this->ui->listWidget->takeItem(index.row());
             }
         }

         );


}
//收信箱操作
void kehuduan::on_listWidget_2_itemClicked(QListWidgetItem *item)
{
    connect(ui->pushButton_8,&QPushButton::clicked,
            [=]()
          {
                 ui->textEdit_2->clear();
               QString tp=item->text();
               this->ui->textEdit_2->setText(tp);
               this->ui->page_5->show();
               this->ui->page_7->hide();
               this->ui->page_6->hide();
               this->ui->page_2->hide();
               this->ui->page_8->hide();
          }
            );
    connect(ui->pushButton_7,&QPushButton::clicked,
            [=](){
        foreach(QModelIndex index,ui->listWidget_2->selectionModel()->selectedIndexes()){

               QString io=item->text();
               this->ui->listWidget_3->addItem(io);
               this->ui->listWidget_2->takeItem(index.row());
            }
             }
            );
    connect(ui->pushButton_17,&QPushButton::clicked,
            [=]()
            {
            ui->textEdit_2->clear();
             QString io=item->text();
             QString  id;
             int j=0;
             for(int i=0;i<io.length();i++)
             {
                 if(io[i]!=' ')
                 {

                     id[j++]=io[i];
                 }
                 else{break;}
             }

             ui->lineEdit->setText(id);
             this->ui->page_2->hide();
             this->ui->page_7->hide();
             this->ui->page_6->hide();
             this->ui->page_8->hide();
              this->ui->page_5->show();

            }
            );




}
//垃圾箱操作
void kehuduan::on_listWidget_3_itemClicked(QListWidgetItem *item)
{
    connect(ui->pushButton_16,&QPushButton::clicked,
            [=]()
           {
        foreach(QModelIndex index,ui->listWidget_3->selectionModel()->selectedIndexes()){
               QString io=item->text();
               this->ui->listWidget_2->addItem(io);
               this->ui->listWidget_3->takeItem(index.row());
        }
            }
            );


}

void kehuduan::on_pushButton_12_clicked()
{
    this->ui->page_5->hide();
    this->ui->page_7->hide();
    this->ui->page_6->hide();
    this->ui->page_8->hide();
     this->ui->page_2->show();
}

void kehuduan::on_pushButton_4_clicked()
{
    this->ui->page_5->hide();
    this->ui->page_7->hide();
    this->ui->page_6->show();
     this->ui->page_2->hide();
    this->ui->page_8->hide();
}
//下载附件
/*void kehuduan::on_pushButton_18_clicked()
{

    isstart1=false;
    tcp->write("06");
     //ui->textEdit_4->setText(fileName2);

}*/
void kehuduan::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
/*void  kehuduan::receivefile()
{
    QString ip  = "172.20.10.12" ;
    qint16  port= 5000 ;
    mygod="kk";
    tcp->abort();
    isstart1=false;
    tcp->connectToHost(ip,port);
    tcp->write("06");

    connect(tcp,&QTcpSocket::readyRead,
            [=]()
            {

            QByteArray pp=tcp->readAll();
            QString qq=QString(pp);
           if(isstart2==true){
          receivesize=0;
          file2.close();
          fileName2=qq.section("#",0,0);
          fileSize2=qq.section("#",1,1).toInt();
          qDebug()<<fileName2<<fileSize2;
          //QString str="G:/textfile/day01/shouxin.txt";
          file2.setFileName(fileName);
         bool isOk=file2.open(QIODevice::WriteOnly);
         if(isOk==false)
         {

             QMessageBox::warning(this,"文件打开错误","错误");
             return;
         }
           isstart2=false;
            }
        else{
            qDebug()<<"hahah";
            qint64 len= file2.write(pp);
            receivesize+=len;
            qDebug()<<len;
            if(receivesize==fileSize2)
            {
                file2.close();
                 tcp->disconnectFromHost();
                QMessageBox::information(this,"完成","文件接收完成");
                return ;
            }
        }


             }



            );


}*/
