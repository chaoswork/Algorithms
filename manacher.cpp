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

typedef long long LL;

const int maxn=5000100;
string org;
string str;
int R[maxn*2];
int np[maxn];

/* manacher algorithm,find longest palindromic
 * substring in O(n) time.
 * R[i] is the longest Radius of palindrome include i*/
void calcR()
{
	/* pre-work,make a string "abc"=>"#a#b#c#"
	 * "abcd"=>"#a#b#c#d#",so the even numbers of
	 * palindrome is easily to solve.*/
	int i;
	str="@";
	for(i=0;i<org.size();++i)
	{
		str+="#";
		str+=org[i];
	}
	str+="#";
	
	
	int mx=0,id=0;
	for(i=1;i<str.size();++i)
	{
		/*     -------id--------
		 *    |                 |
		 *----mn--j----*----i--mx---
		 *               |     |
		 *   j=id*2-i    -------		 *
		 * */
		R[i]=mx>i?min(mx-i,R[id*2-i]):1;
		while(i+R[i]<str.size() && i-R[i]>=0 && 
				str[i+R[i]]==str[i-R[i]]) ++R[i];
		if(mx<i+R[i])
		{
			mx=i+R[i];
			id=i;
		}
	}
}
int main(int argc, char *argv[])
{
#ifdef CHAOS
	freopen("data.in","r",stdin);
#endif
	cin>>org;
	calcR();
	int n=str.size();
	int i;
	for(i=0;i<n;++i)
		cout<<i%10<<" ";
	cout<<endl;
	for(i=0;i<n;++i)
		cout<<str[i]<<" ";
	cout<<endl;
	for(i=0;i<n;++i)
		cout<<R[i]<<" ";
	cout<<endl;
	return 0;
}
