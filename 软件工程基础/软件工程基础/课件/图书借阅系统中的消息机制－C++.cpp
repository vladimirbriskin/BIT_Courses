#include <iostream>
#include <string>
#include <map>
#include <list>

using namespace std;

class BookItem;                             // 向前引用
typedef void (*BOOK_AVAIABLE) (BookItem);   // 发送消息的函数指针

class BookItem
{
public:
	string BookName;
	int BookNumber;

	BookItem(string bookName = "", int num = 0)
	{
		BookName = bookName;
		BookNumber = num;
	}
};

class BookItemList
{
private:
	map<string ,BookItem> Books;      // 图书列表
public:
	BookItemList() { }

	bool RemoveBook(BookItem oneBook)
	{
		if (!Find(oneBook.BookName)) return false;

		Books[oneBook.BookName].BookNumber--;

		// 将该书从图书列表中删除
		if (Books[oneBook.BookName].BookNumber == 0) 
		   Books.erase(oneBook.BookName);

		return true;
	}

	void AddBook(BookItem oneBook)
	{
		if (!Find(oneBook.BookName))  // 增加的书不在书库中
		{
			Books.insert(make_pair(oneBook.BookName, oneBook));  // 新书入库
		}
		
		Books[oneBook.BookName].BookNumber++;
	}

	bool Find(string bookName)  // 判断一本书是否在书库中
	{ 
		return Books.find(bookName) != Books.end();
	}
};

class Library
{
private:
	static BookItemList AllBooks;      // 图书馆的书库
public:	
	static BOOK_AVAIABLE pfBookIsOK;   // 发送消息的函数指针，定义为静态变量，是因为函数指针是指向全局函数的指针。

	void BuyNewBook(BookItem oneBook)  // 购买新书
	{
		AllBooks.AddBook(oneBook);
		//if (!pfBookIsOK)  pfBookIsOK(oneBook);           // 对订阅了该书的人发出通知
	}
	bool Find(string bookName)
	{
		return AllBooks.Find(bookName);
	}

	void SendMessage(BookItem oneBook)
	{
		pfBookIsOK(oneBook);

		// 已发送消息，故而重设函数指针
		pfBookIsOK = NULL;
	}
};
BOOK_AVAIABLE Library::pfBookIsOK = NULL;
BookItemList Library::AllBooks;

class Reader
{
private:
	string ReaderName;  // 读者姓名
	Library& MyLib;      // 图书馆

	list<string> listSubscribe;  // 读者待借阅的书	

	// 定义为静态函数，是为了配合函数指针
	// 定义为私有成员函数，是因为：
	// 从用户角度说，用户不用主动订阅。但需要借的书没有的时候，自动订阅。
	static void SubscribeBook(BookItem oneBook)
	{
		cout << "书已到，给你发邮件了！" << endl;
	}
public:
	Reader(string name, Library& L) : MyLib(L) { ReaderName = name; }

	// 借书
	void BorrowBook(BookItem& oneBook)   
	{
		if (MyLib.Find(oneBook.BookName) && oneBook.BookNumber > 0)  // 图书馆有该书，可以借
		{
			// 借书
			oneBook.BookNumber--;
		}
		else // 图书馆中无书，则预订
		{
			cout << "《" << oneBook.BookName << "》已没有！" << endl << endl;

			// 将所借而未借到的书放入预订书列表中
			listSubscribe.push_back(oneBook.BookName);

			// 添加预订书的消息			
			MyLib.pfBookIsOK = SubscribeBook;
		}
	}

	// 还书
	void ReturnBook(BookItem oneBook)
	{
		// pfBookIsOK不为空，说明有人预订了该书
		if (MyLib.pfBookIsOK != NULL)
		{
			MyLib.SendMessage(oneBook);       // 通知订阅了该书的其他读者
		}
	}
};

void main()
{
	// 设置书名及数量
	BookItem BI("软件工程基础", 1);

	// 初始化当前书库，设置书库中有“软件工程基础”一书，且仅有一本。
	Library Lib;
	Lib.BuyNewBook(BI);


	// 定义读者对象（相当于：读者注册）
	Reader R1("ZhangSan", Lib);
	Reader R2("LiSi", Lib);

	cout << "张三借书" << endl << endl;
	R1.BorrowBook(BI);

	cout << "李四借书" << endl;
	R2.BorrowBook(BI);

	cout << "张三还书" << endl << endl;
	R1.ReturnBook(BI);
}