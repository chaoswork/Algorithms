#include  <cstdio>
#include  <iostream>
#include  <algorithm>
using namespace std;

//write a quicksort to remember my first interview,Tencent

void qsort(int *a,int s,int e)
{
	if(s>=e) return ;
	int key=a[e];
	int idx=s-1;//a[0...idx]<=key;a[idx+1...e]>key
	int i;
	for(i=s;i<e;++i)
	{
		if(a[i]<=key)
			swap(a[++idx],a[i]);	
	}
	swap(a[++idx],a[e]);
	qsort(a,s,idx-1);
	qsort(a,idx+1,e);
}

int main(int argc, char *argv[])
{
	int a[1024];
	int n;
	while(cin>>n)
	{
		int i;
		for(i=0;i<n;++i)
			cin>>a[i];
		qsort(a,0,n-1);
		for(i=0;i<n;++i)
			cout<<a[i]<<" ";
		cout<<endl;
	}
	return 0;
}
