#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

vector<vector<int>> buffers,passes;
vector<int> input;
int min (int a, int b) {return (a<=b) ? a : b;}

void merge (int beg, int len)
{
	map<int,int> pos;
	for (int i=beg; i<beg+len; i++) pos.insert(make_pair(i,0));
	vector<int> output;
	while (pos.size()>1)
	{
		int ele=buffers[pos.begin()->first][pos.begin()->second];
		map<int,int>::iterator arrpos=pos.begin();
		for (map<int,int>::iterator i=pos.begin(); i!=pos.end(); i++)
		{
			if (ele>buffers[i->first][i->second])
			{
				ele=buffers[i->first][i->second];
				arrpos=i;
			}
		}
		output.push_back(ele);
		if (arrpos->second==buffers[arrpos->first].size()-1)
			pos.erase(arrpos->first);
		else (arrpos->second)++;
	}
	while (pos.begin()->second<buffers[pos.begin()->first].size())
	{
		output.push_back(buffers[pos.begin()->first][pos.begin()->second]);
		(pos.begin()->second)++;
	}
	passes.push_back(output);
}

int main ()
{
	int buffer_pages,record_count,total_records,temp;
	cin>>buffer_pages>>record_count>>total_records;
	input.resize(total_records);
	for (int i=0; i<total_records; i++) scanf ("%d ",&input[i]);
	int j=-1;
	for (int i=0; i<input.size(); i++)
	{
		if (i%record_count==0)
		{
			vector<int> arr;
			arr.push_back(input[i]);
			buffers.push_back(arr);
			j++;
		}
		else buffers[j].push_back(input[i]);
	}
	for (int i=0; i<buffers.size(); i++) sort (buffers[i].begin(),buffers[i].end());
	int pptr=0;
	while (pptr<buffers.size())
	{
		merge (pptr,min(buffer_pages,buffers.size()-pptr));
		pptr+=buffer_pages;
	}
	while (passes.size()>1)
	{
		buffers.clear();
		for (int i=0; i<passes.size(); i++) buffers.push_back(passes[i]);
		passes.clear();
		int pptr=0;
		while (pptr<buffers.size())
		{
			merge (pptr,min(buffer_pages-1,buffers.size()-pptr));
			pptr+=buffer_pages-1;
		}
	}
	buffers.clear();
	for (int i=0; i<passes[0].size(); i++) printf ("%d ",passes[0][i]);
	cout<<endl;
	return 0;
}