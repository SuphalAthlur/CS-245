#include <iostream>
#include <stack>
using namespace std;

int nonleaf_size=1,leaf_size=2;
int level;

class node
{
private:
	bool type;
	node** ptrlist;
	node *next_node,*prev_node;
	int *valuelist,*keylist;
	int occupancy;

public:
	node (bool reqd);
	int bs (int key);
	bool insert (int key, node* smalladdr, node* bigaddr);
	void init (int* nums, node** connect, int max);
	pair < int, pair < node*, node* > > splitter ();

	bool identify() {return type;};
	bool ret_value(int pos) {return (type) ? keylist[pos] : valuelist[pos];};
	node* ret_addr(int pos) {return ptrlist[pos];};
	void prev(node* inp) {prev_node=inp;};
	void next(node* inp) {next_node=inp;};
	void display ();
	void show ()
	{
		int* reqdlist = (type) ? keylist : valuelist;
		for (int i=0; i<occupancy; i++) cout<<reqdlist[i]<<" ";
		cout<<endl;
	}
};
stack<node*> ooo;
typedef pair < int, pair < node*, node* > > container;

class tree
{
private:
	node *root,*start;
	int size,index_nodes,data_nodes;

public:
	tree ();
	void insert (int key);
	bool search (int key);

	void update_start (node* inp) {start=inp;};
	void output ()
	{
		cout<<index_nodes<<" "<<data_nodes<<endl;
		root->display();
	};
	void display ()
	{
		level=0;
		cout<<index_nodes<<" "<<data_nodes<<" ";
		root->show();
	};
}*parent;

int main ()
{
	parent=new tree;
	cin>>leaf_size>>nonleaf_size;
	int choice,entry;
	cin>>choice;
	while (choice!=3)
	{
		if (choice==1)
		{
			cin>>entry;
			parent->insert(entry);
		}
		else if (choice==2)
		{
			parent->display();
		}
		else if (choice!=3)
		{
			cout<<"Invalid demand, try again\n";
		}
		cin>>choice;
	}
	while (!ooo.empty())
	{
		free(ooo.top());
		ooo.pop();
	}
	delete parent;
	return 0;
}

node::node (bool reqd)
{
	if (reqd)
	{
		ptrlist = (node**) malloc((2*nonleaf_size+3)*sizeof(node*));
		keylist = (int*) malloc((2*nonleaf_size+2)*sizeof(int));
	}
	else
	{
		valuelist = (int*) malloc((2*leaf_size+2)*sizeof(int));
	}
	type=reqd;
	occupancy=0;
	next_node=prev_node=NULL;
}

int node::bs (int key)
{
	int *reqdlist;
	if (type) reqdlist=keylist;
	else reqdlist=valuelist;
	int l=0,r=occupancy-1;
	int m;
	while (l<r)
	{
		m=(l+r)/2;
		if (key<reqdlist[m])
		{
			r=m;
		}
		else
		{
			l=m+1;
		}
	}
	if (l==occupancy-1 && reqdlist[l]<=key) l++;
	return l;
}


bool node::insert (int key, node* smalladdr, node* bigaddr)
{
	int *reqdlist;
	if (type) reqdlist=keylist;
	else reqdlist=valuelist;
	int pos=bs(key);
	int ptr=occupancy;
	while (ptr>pos)
	{
		reqdlist[ptr]=reqdlist[ptr-1];
		if (type) ptrlist[ptr+1]=ptrlist[ptr];
		ptr--;
	}
	reqdlist[pos]=key;
	if (type)
	{
		ptrlist[pos+1]=bigaddr;
		ptrlist[pos]=smalladdr;
	}
	occupancy++;
	if (type && occupancy==2*nonleaf_size+2) return true;
	else if (!type && occupancy==2*leaf_size+1)
	{
		return true;
	}
	return false;
}

void node::init (int* nums, node** connect, int max)
{
	int* chain;
	occupancy=max;
	if (type) chain=keylist;
	else chain=valuelist;
	for (int i=0; i<max; i++)
	{
		chain[i]=nums[i];
		if (type) ptrlist[i]=connect[i];
	}
	if (type) ptrlist[max]=connect[max];
	return;
}

container node::splitter ()
{
	node *part1=new node(type);
	node *part2=new node(type);
	int mid;
	if (type)
	{
		int arr[nonleaf_size+1];
		node *links[nonleaf_size+2];
		for (int i=0; i<nonleaf_size; i++)
		{
			arr[i]=keylist[i];
			links[i]=ptrlist[i];
		}
		links[nonleaf_size]=ptrlist[nonleaf_size];
		(*part1).init(arr,links,nonleaf_size);
		mid=keylist[nonleaf_size];
		for (int i=nonleaf_size+1; i<2*nonleaf_size+2; i++)
		{
			arr[i-nonleaf_size-1]=keylist[i];
			links[i-nonleaf_size-1]=ptrlist[i];
		}
		links[nonleaf_size+1]=ptrlist[2*nonleaf_size+2];
		(*part2).init(arr,links,nonleaf_size+1);
	}
	else
	{
		int arr[leaf_size+1];
		for (int i=0; i<leaf_size; i++)
		{
			arr[i]=valuelist[i];
		}
		(*part1).init(arr,NULL,leaf_size);
		// mid=(valuelist[leaf_size]+valuelist[leaf_size-1]+1)/2;
		mid=valuelist[leaf_size];
		for (int i=leaf_size; i<2*leaf_size+1; i++)
		{
			arr[i-leaf_size]=valuelist[i];
		}
		(*part2).init(arr,NULL,leaf_size+1);
		(*part1).prev(prev_node);
		(*part1).next(part2);
		(*part2).prev(part1);
		(*part2).next(next_node);
		if (prev_node==NULL) parent->update_start(part1);
	}
	return make_pair(mid,make_pair(part1,part2));
}

void node::display ()
{
	cout<<"Layer "<<level<<": "; show();
	if (!identify()) return; 
	level++;
	for (int i=0; i<occupancy+1; i++)
	{
			ptrlist[i]->display();
	}
	level--;
}

tree::tree ()
{
	start=new node(false);
	root=start;
	size=index_nodes=data_nodes=0;
}

bool tree::search (int key)
{
	node* stp=root;
	while (stp->identify()) stp=stp->ret_addr(stp->bs(key));
	int pos=stp->bs(key);
	if (pos==0) return false;
	if (stp->ret_value(pos-1)==key) return true;
	return false;
}

void tree::insert (int key)
{
	if (root==start)
	{
		bool status=start->insert(key,NULL,NULL);
		container holder;
		if (data_nodes==0) data_nodes++;
		if (status)
		{
			holder=start->splitter();
			ooo.push(start);
			root=new node(true);
			int arr[1]={holder.first};
			node* connect[2]={holder.second.first,holder.second.second};
			root->init(arr,connect,1);
			data_nodes++;
			index_nodes++;
		}
	}
	else
	{
		stack<node*> locs;
		node* trav=root;
		while (trav->identify())
		{
			locs.push(trav);
			trav=trav->ret_addr(trav->bs(key));
		}
		locs.push(trav);
		bool status;
		int ins=key;
		node *a=NULL,*b=NULL;
		while (!locs.empty())
		{
			node* temp=locs.top();
			status=temp->insert(ins,a,b);
			if (!status)
			{
				break;
			}
			locs.pop();
			container r=temp->splitter();
			ooo.push(temp);
			if (temp->identify()) index_nodes++;
			else data_nodes++;
			ins=r.first;
			a=r.second.first;
			b=r.second.second;
		}
		if (locs.empty())
		{
			root=new node(true);
			index_nodes++;
			int arr[1]={ins};
			node* connect[2]={a,b};
			root->init(arr,connect,1);				
		}
	}
	size++;
}
