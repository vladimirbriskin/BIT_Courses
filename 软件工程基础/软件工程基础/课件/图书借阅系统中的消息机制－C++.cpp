#include <iostream>
#include <string>
#include <map>
#include <list>

using namespace std;

class BookItem;                             // ��ǰ����
typedef void (*BOOK_AVAIABLE) (BookItem);   // ������Ϣ�ĺ���ָ��

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
	map<string ,BookItem> Books;      // ͼ���б�
public:
	BookItemList() { }

	bool RemoveBook(BookItem oneBook)
	{
		if (!Find(oneBook.BookName)) return false;

		Books[oneBook.BookName].BookNumber--;

		// �������ͼ���б���ɾ��
		if (Books[oneBook.BookName].BookNumber == 0) 
		   Books.erase(oneBook.BookName);

		return true;
	}

	void AddBook(BookItem oneBook)
	{
		if (!Find(oneBook.BookName))  // ���ӵ��鲻�������
		{
			Books.insert(make_pair(oneBook.BookName, oneBook));  // �������
		}
		
		Books[oneBook.BookName].BookNumber++;
	}

	bool Find(string bookName)  // �ж�һ�����Ƿ��������
	{ 
		return Books.find(bookName) != Books.end();
	}
};

class Library
{
private:
	static BookItemList AllBooks;      // ͼ��ݵ����
public:	
	static BOOK_AVAIABLE pfBookIsOK;   // ������Ϣ�ĺ���ָ�룬����Ϊ��̬����������Ϊ����ָ����ָ��ȫ�ֺ�����ָ�롣

	void BuyNewBook(BookItem oneBook)  // ��������
	{
		AllBooks.AddBook(oneBook);
		//if (!pfBookIsOK)  pfBookIsOK(oneBook);           // �Զ����˸�����˷���֪ͨ
	}
	bool Find(string bookName)
	{
		return AllBooks.Find(bookName);
	}

	void SendMessage(BookItem oneBook)
	{
		pfBookIsOK(oneBook);

		// �ѷ�����Ϣ���ʶ����躯��ָ��
		pfBookIsOK = NULL;
	}
};
BOOK_AVAIABLE Library::pfBookIsOK = NULL;
BookItemList Library::AllBooks;

class Reader
{
private:
	string ReaderName;  // ��������
	Library& MyLib;      // ͼ���

	list<string> listSubscribe;  // ���ߴ����ĵ���	

	// ����Ϊ��̬��������Ϊ����Ϻ���ָ��
	// ����Ϊ˽�г�Ա����������Ϊ��
	// ���û��Ƕ�˵���û������������ġ�����Ҫ�����û�е�ʱ���Զ����ġ�
	static void SubscribeBook(BookItem oneBook)
	{
		cout << "���ѵ������㷢�ʼ��ˣ�" << endl;
	}
public:
	Reader(string name, Library& L) : MyLib(L) { ReaderName = name; }

	// ����
	void BorrowBook(BookItem& oneBook)   
	{
		if (MyLib.Find(oneBook.BookName) && oneBook.BookNumber > 0)  // ͼ����и��飬���Խ�
		{
			// ����
			oneBook.BookNumber--;
		}
		else // ͼ��������飬��Ԥ��
		{
			cout << "��" << oneBook.BookName << "����û�У�" << endl << endl;

			// �������δ�赽�������Ԥ�����б���
			listSubscribe.push_back(oneBook.BookName);

			// ���Ԥ�������Ϣ			
			MyLib.pfBookIsOK = SubscribeBook;
		}
	}

	// ����
	void ReturnBook(BookItem oneBook)
	{
		// pfBookIsOK��Ϊ�գ�˵������Ԥ���˸���
		if (MyLib.pfBookIsOK != NULL)
		{
			MyLib.SendMessage(oneBook);       // ֪ͨ�����˸������������
		}
	}
};

void main()
{
	// ��������������
	BookItem BI("������̻���", 1);

	// ��ʼ����ǰ��⣬����������С�������̻�����һ�飬�ҽ���һ����
	Library Lib;
	Lib.BuyNewBook(BI);


	// ������߶����൱�ڣ�����ע�ᣩ
	Reader R1("ZhangSan", Lib);
	Reader R2("LiSi", Lib);

	cout << "��������" << endl << endl;
	R1.BorrowBook(BI);

	cout << "���Ľ���" << endl;
	R2.BorrowBook(BI);

	cout << "��������" << endl << endl;
	R1.ReturnBook(BI);
}