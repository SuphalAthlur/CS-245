#include <iostream>
#include <map>
using namespace std;

class page
{
	int count,freepoint;
	page *previous,*next;
	map<int,int> directory;

	map<int,int> width,history;
	int available;

public:
	void init(int, page*, page*);
	bool insert(int, int);
	int find (int);
	void next_page(page* ptr) {next=ptr;};
	int record_count() {return count;};
	page* ahead() {return next;};
	page* behind() {return previous;};
};

class book
{
	page *start,*end;
	int page_capacity;
	int page_count;

public:
	book(int);
	void insert(int, int);
	pair<int,int> search(int);
	void display();
	void clear();
};

int main ()
{
	int byte_size;
	cin>>byte_size;
	book db(byte_size);

	int choice,size,key;
	cin>>choice;
	pair<int,int> ans;
	while (choice!=4)
	{
		switch (choice)
		{
			case 1:
				cin>>size>>key;
				db.insert(size,key);
				break;
		
			case 2:
				db.display();
				break;
		
			case 3:
				cin>>key;
				ans=db.search(key);
				printf ("%d %d\n",ans.first,ans.second);
				break;
		
			case 4:
				db.clear();
				break;
		
			default:
				cout<<"Invalid choice :)\nTry again\n";
		}
		cin>>choice;
	}
	return 0;
}

void page::init(int size, page* p, page* n)
{
	count=freepoint=0;
	previous=p;
	next=n;
	available=size-16;
}

bool page::insert(int space, int pkey)
{
	if (available<space+4) return false;
	history.insert(make_pair(pkey,count));
	directory.insert(make_pair(pkey,freepoint));
	width.insert(make_pair(pkey,space));
	freepoint+=space;
	available-=(4+space);
	count++;
	return true;
}

int page::find(int key)
{
	if (directory.find(key)==directory.end()) return -1;
	else return history[key];
}

book::book(int size)
{
	start=end=NULL;
	page_count=0;
	page_capacity=size;
}

void book::insert(int space, int pkey)
{
	if (start==NULL)
	{
		page* born=new page;
		start=born;
		end=born;
		start->init(page_capacity,NULL,NULL);
		bool hold=start->insert(space,pkey);
		page_count++;
		return;
	}
	page* trav=start;
	while (trav!=NULL)
	{
		if (trav->insert(space,pkey)) break;
		trav=trav->ahead();
	}
	if (trav==NULL)
	{
		page* born=new page;
		born->init(page_capacity,end,NULL);
		end->next_page(born);
		end=born;
		page_count++;
		bool hold=end->insert(space,pkey);
	}
	return;
}

pair<int,int> book::search (int key)
{
	page* trav=start;
	int page_id=0,slot_id=-1;
	while (trav!=NULL)
	{
		slot_id=trav->find(key);
		if (slot_id!=-1) break;
		trav=trav->ahead();
		page_id++;
	}
	if (slot_id==-1) return make_pair(-1,-1);
	else return make_pair(page_id,slot_id);
}

void book::display()
{
	printf ("%d ",page_count);
	page* trav=start;
	while (trav!=NULL)
	{
		printf ("%d ",trav->record_count());
		trav=trav->ahead();
	}
	printf ("\n");
	return;
}

void book::clear()
{
	page* trav=start;
	while (trav!=NULL)
	{
		page* temp=trav->ahead();
		delete trav;
		trav=temp;
	}
	return;
}