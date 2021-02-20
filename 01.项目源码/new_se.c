#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>
#include<string.h>
#include<mysql/mysql.h>
#include<fstream>
#include<string>
#include<iostream>

using namespace std;
#define MAXLINE 4096
#define SERV_PORT 5000
 
 MYSQL *conn;
MYSQL_RES *res;//结果集
MYSQL_RES *res2;
MYSQL_ROW row;//存储的一条数据
MYSQL_ROW row1;
 
 char recv_buff[4096];
  char send_buff[4096];
 char batmp[10000];//back函数
 char nowid[20];
 char filename[30];
 
struct s_info 
{
	struct sockaddr_in cliaddr;
	int connfd;
};

void savesql(char a[],char b[],char c[],char g[])//recver4,title4,content4, attname4,attsize4,attcontent4,sender4,发到服务器
{
	char saql[10000]="insert ignore into EmailTable(sendtargetID,title,content,userid)values('";
	strcat(saql,a);
	strcat(saql,"','");
	strcat(saql,b);
	strcat(saql,"','");
	strcat(saql,c);
	//strcat(saql,"','");
	/*strcat(saql,d);
	strcat(saql,"','");
	strcat(saql,e);
	strcat(saql,"','");
	strcat(saql,f);*/
	strcat(saql,"','");
	strcat(saql,g);
	strcat(saql,"')");
	int l1=strlen(saql);
	saql[l1]=' ';
	
	if(mysql_query(conn,saql))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		mysql_close(conn);
		exit(1);
	}
}

char* back(char* eid)//返回
{	
	memset(batmp,0,sizeof(batmp));
	char bac[10000]="select userid,title,content from EmailTable where sendtargetID='";
	strcat(bac,nowid);
	strcat(bac,"' and emailID='");
	strcat(bac,eid);
	strcat(bac,"'");
	if(mysql_query(conn,bac))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		mysql_close(conn);
		exit(1);
	}
	res2=mysql_store_result(conn);
	if((row1=mysql_fetch_row(res2))!=NULL)
	{
		strcat(batmp,row1[0]);
		strcat(batmp,"#");
		strcat(batmp,row1[1]);
		strcat(batmp,"#");
		strcat(batmp,row1[2]);
		return batmp;
	}
}
void pullsql(int le)
{
	char pl[200]="select emailID from EmailTable where sendtargetID='";
	strcat(pl,nowid);
	strcat(pl,"'");
	if(mysql_query(conn,pl))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		mysql_close(conn);
		exit(1);
	}
	res=mysql_store_result(conn);
	while((row=mysql_fetch_row(res))!=NULL)
	{
		write(le,back(row[0]),MAXLINE);	
	}
	//char* kk="@";
	//write(le,kk,MAXLINE);
}





char* findsql(char a[],char b[])//a是用户，b是电话
{
	char fin[100]="select password from login where username='";
	strcat(fin,a);
	strcat(fin,"' and TEL='");
	strcat(fin,b);
	strcat(fin,"'");
	if(mysql_query(conn,fin))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		mysql_close(conn);
		exit(1);
	}
	res=mysql_store_result(conn);
	if((row=mysql_fetch_row(res))!=NULL)
	{
		//if(res!=NULL) mysql_free_result(res);
		return row[0];
	}
}
int regissql(char a[],char b[],char c[])//a是用户，b是密码,c是电话号
{
	char sel[100]="insert ignore into login(username,password,TEL)values('";
	strcat(sel,a);
	strcat(sel,"','");
	strcat(sel,b);
	strcat(sel,"','");
	strcat(sel,c);
	strcat(sel,"')");
	int l1=strlen(sel);
	sel[l1]=' ';
	
	if(mysql_query(conn,sel))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		mysql_close(conn);
		exit(1);
	}
	if(mysql_affected_rows(conn)) 
	{
		return 1;}
	else 
	{
		return 0;}
	
}
int loginsql(char a[],char b[])//a是用户，b是密码
{
	char tmp[100]="select password from login where username='";
	strcat(tmp,a);
	memset(nowid,0,sizeof(nowid));
	strcpy(nowid,a);
	int ll=strlen(tmp);
	tmp[ll]='\'';
	if(mysql_query(conn,tmp))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		mysql_close(conn);
		exit(1);
	}
	res=mysql_use_result(conn);
	if((row=mysql_fetch_row(res))!=NULL)
	{
		if(strcmp(row[0],b)==0) 
		{if(res!=NULL) mysql_free_result(res);
				return 1;}
		else 
		{if(res!=NULL) mysql_free_result(res);
				return 0;}
	}
	else 
	{if(res!=NULL) mysql_free_result(res);	
	return 0;}
}

void chk(char a[],int leixing,int daxiao)
{
	int len=strlen(a);
	
	if(a[0]=='0'&&a[1]=='0')//登录检查
	{	printf("00");
		char username1[20];
		char password1[20];
		int i;
		int puser=0;
		for(i=2;i<len;i++)
		{
			if(a[i]=='#') break;
			else
			{
				username1[puser]=a[i];
				puser++;
			}
		}
		username1[puser]='\0';//看看是不是需要删除
		i++;
		int ppass=0;
		for(;i<len;i++)
		{
			password1[ppass]=a[i];
			ppass++;
		}
		password1[ppass]='\0';//看看是不是需要删除
		if(loginsql(username1,password1))  write(leixing,"1",MAXLINE);
		else write(leixing,"0",MAXLINE);	
	}
	else if(a[0]=='0'&&a[1]=='1')//注册
	{
		printf("01");
		int i;
		int puser=0;
		char username2[20];
		char password2[20];
		char tel2[20];
		for(i=2;i<len;i++)
		{
			if(a[i]=='#') break;
			else
			{
				username2[puser]=a[i];
				puser++;
			}
		}
		username2[puser]='\0';//看看是不是需要删除
		i++;
		int ppass=0;
		for(;i<len;i++)
		{
			if(a[i]=='#') break;
			else
			{
			password2[ppass]=a[i];
			ppass++;
			}
		}
		password2[ppass]='\0';//看看是不是需要删除
		i++;
		for(;i<len;i++)
		{
			if(a[i]=='#') break;
		}
		i++;
		int ptel=0;
		for(;i<len;i++)
		{
			tel2[ptel]=a[i];
			ptel++;
		}
		tel2[ptel]='\0';//看看是不是需要删除
		if(regissql(username2,password2,tel2)) write(leixing,"1",MAXLINE);
		else  write(leixing,"0",MAXLINE);	
	}
	else if(a[0]=='0'&&a[1]=='2')//找回密码
	{
		printf("02");
		char username3[20];
		char tel3[20];
		int i;
		int puser=0;
		for(i=2;i<len;i++)
		{
			if(a[i]=='#') break;
			else
			{
				username3[puser]=a[i];
				puser++;
			}
		}
		username3[puser]='\0';//看看是不是需要删除
		i++;
		int ptel=0;
		for(;i<len;i++)
		{
			tel3[ptel]=a[i];
			ptel++;
		}
		tel3[ptel]='\0';//看看是不是需要删除
		write(leixing,findsql(username3,tel3),MAXLINE);
		
	}
	else if(a[0]=='0'&&a[1]=='3')//发邮件到服务端
	{
		printf("03");
		char recver4[20];//
		char title4[30];//
		char content4[4096];//
		//char attname4[30];//
		//char attsize4[10];//
		//char attcontent4[4096];//
		char sender4[20];//
		strcpy(sender4,nowid);
		int i;
		int prec=0;
		for(i=2;i<len;i++)
		{
			if(a[i]=='#') break;
			else
			{
				recver4[prec]=a[i];
				prec++;
			}
		}
		recver4[prec]='\0';//看看是不是需要删除
		i++;
		
		
		int ptit;
		for(;i<len;i++)
		{
			if(a[i]=='#') break;
			else
			{
				title4[ptit]=a[i];
				ptit++;
			}
		}
		title4[ptit]='\0';
		i++;
		
		
		int pcon=0;
		for(;i<len;i++)
		{
			if(a[i]=='#') break;
			else
			{
				content4[pcon]=a[i];
				pcon++;
			}
		}
		content4[pcon]='\0';
		//i++;
		
		/*int patn=0;
		for(;i<len;i++)
		{
			if(a[i]=='#') break;
			else
			{
				attname4[patn]=a[i];
				patn++;
			}
		}
		attname4[patn]='\0';
		i++;
		
		int pats=0;
		for(;i<len;i++)
		{
			if(a[i]=='#') break;
			else
			{
				attsize4[pats]=a[i];
				pats++;
			}
		}
		attsize4[pats]='\0';
		i++;
		
		
		
		int pato=0;
		for(;i<len;i++)
		{
			attcontent4[pato]=a[i];
			pato++;
		}
		attcontent4[pato]='\0';//看看是不是需要删除
		*/
		//write(leixing,findsql(username3,tel3),MAXLINE);
		//savesql(recver4,title4,content4,attname4,attsize4,attcontent4,sender4);
		savesql(recver4,title4,content4,sender4);
	}
	else if(a[0]=='0'&&a[1]=='4')//返回邮件,有问题，在重写
	{
		pullsql(leixing);
	}
	else if(a[0]=='0'&&a[1]=='5')//客户端到服务端年发附件
	{
		memset(filename,0,sizeof(filename));
		int i;
		//char filename[30];
		char filesize[10];//到底传来的是什么，如果不是字符串应该在另一边改一下
		int pfl=0;
		for(i=2;i<len;i++)
		{
			if(a[i]=='#') break;//可能出错
			else
			{
				filename[pfl]=a[i];
				pfl++;
			}
		}
		filename[pfl]='\0';
		i++;
		
		int psz;
		for(;i<len;i++)
		{
			if(a[i]=='#') break;//可能出错
			else
			{
				filesize[psz]=a[i];
				psz++;
			}
		}
		filesize[psz]='\0';
		
		int fxs=atoi(filesize);
		
		
		
		if(a[i]=='#') //receatt(filename,filesize,leixing);
		{
			
			i++;
			std::fstream fs;
			string idd=filename;
			fs.open((idd).c_str(),std::fstream::out|std::fstream::binary|std::fstream::trunc);
			if(!fs) printf("文件没找到啊\n");
			int fen;
			
			while(fxs)//文件大小
			{
				fen=recv(leixing,recv_buff,MAXLINE,0);
				if(fen>0)//读入的大小
				{
					fs.write(recv_buff,fen);
					fxs-=fen;
				}
				printf("收到一个包\n");
				
			
			}
			fs.close();
		}
		//memset(filename,0,sizeof(filename));
		
	}
	
	else if(a[0]=='0'&&a[1]=='6')//
	{
		std::fstream fs1;
		string idf=filename;
		fs1.open((idf).c_str(),std::fstream::in|std::fstream::binary);
		fs1.seekg(0,std::fstream::end);
		int Fsize=fs1.tellg();//得到大小
		char num[20];
		//itoa(Fsize,num,10);
		sprintf(num,"%d",Fsize);
		string NUM=num;
		fs1.seekg(0,std::fstream::beg);
		string myst;
		myst=idf+"#"+NUM;
		
		std::cout<<myst<<std::endl;
		send(leixing,myst.c_str(),myst.size()+1,0);
		
		usleep(80000);  
		while(!fs1.eof())
		{
			//usleep(80000);
			printf("%s",send_buff);
			fs1.read(send_buff,MAXLINE);
			int ll=fs1.gcount();
			send(leixing,send_buff,ll,0);
			printf("发去一个包\n");
		}
		fs1.close();
		
	}
	
	
	
	
	
	
	
	
}


void *do_work(void *arg)
{
	int n,i;
	char reply[MAXLINE];
	struct s_info *ts = (struct s_info*)arg;
	char buf[100000];//在线程自己的用户空间栈开辟的,该线程运行结束的时候,主控线程就不能操作这块内存了
	char rebuf[MAXLINE];
	char str[INET_ADDRSTRLEN];//INET_ADDRSTRLEN 是宏16个字节
 	
	//在创建线程前设置线程创建属性,设为分离态,效率高
	pthread_detach(pthread_self());
 
	while (1) 
	{
	n = read(ts->connfd, buf, MAXLINE);
	if (n == 0) 
	{
		printf("the other side has been closed.\n");
		break;
	}
	printf("received from %s at PORT %d\n",
	inet_ntop(AF_INET, &(*ts).cliaddr.sin_addr, str, sizeof(str)),ntohs((*ts).cliaddr.sin_port));
 	printf("buf:%s\n",buf);
	chk(buf,ts->connfd,n);	
	memset(buf,0,sizeof(buf));
	}
	
	close(ts->connfd);
}
int main(void)
{

	
	char server[]="localhost";
	char user[]="root";
	char password[]="123456";
	char database[]="members";
	conn = mysql_init(NULL);
	
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		exit(1);
	}
	
	
	
	//mysql_close(conn);
	
	//printf("finish!\n");
/************************************分界线，上面是数据库*************************************/
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	int i = 0;
	pthread_t tid;
	struct s_info ts[3497];
 
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
 
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, 20);
 
	printf("Accepting connections ...\n");
/***********************************上面一小段应该没问题*************************************/
	while (1) 
	{
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
		ts[i].cliaddr = cliaddr;
		ts[i].connfd = connfd;
/* 达到线程最大数时，pthread_create出错处理, 增加服务器稳定性 */
		pthread_create(&tid, NULL, do_work, (void*)&ts[i]);//把accept得到的客户端信息传给线程，让线程去和客户端进行数据的收发
		i++;
	}
	return 0;
}
