#include<iostream>
#include<vector>
#include<cstring>
#include<string>
#include<map>
#include<set>
#include<algorithm>
#include<stdlib.h>
#include<stdio.h>

using namespace std;


const int maxn=100000+10;
int Next[maxn];
void getNext(const string& m)
{
	int i=0;
	int j=-1;
	Next[0]=-1;
	while(i<m.size())
	{
		while(j>=0 && m[i]!=m[j]) j=Next[j];
		++i;
		++j;
		Next[i]=j;
	}
}
void kmp(const string& s,const string& m)
{
	int i=0;
	int j=0;
	while(i<s.size())
	{
		while(j>=0 && s[i]!=m[j]) j=Next[j];
		++i;
		++j;
		if(j==m.size()) 
		{
			cout<<i-j<<endl;
			j=Next[j];
		}
	}
}

