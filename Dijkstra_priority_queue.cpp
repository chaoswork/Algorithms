#include<iostream>
#include<vector>
#include<cstring>
#include<string>
#include  <queue>
#include<map>
#include<set>
#include<algorithm>
#include<stdlib.h>
#include<stdio.h>

using namespace std;
/* codeforces 20c */
typedef long long LL;
int n,m;

const int maxn=100000+10;
const LL INF=((LL)1)<<60;
vector<pair<int,int> > adj[maxn];
priority_queue<pair<LL,int> > pq;//dist,v
LL dist[maxn];
int pre[maxn];

void Dijkstra(int s,int t)
{
	int i;
	for(i=0;i<maxn;++i)
		dist[i]=INF;

	pq.push(make_pair(-(dist[s]=0),s));

	while(!pq.empty())
	{
		int cur=pq.top().second;
		LL cost=-pq.top().first;
		pq.pop();

		if(cost>dist[cur]) continue;
		for(vector<pair<int,int> >::iterator it=adj[cur].begin();
				it!=adj[cur].end();++it)
		{
			int v=it->first;
			int w=it->second;
			if(cost+w<dist[v])
			{
				pq.push(make_pair(-(dist[v]=cost+w),v));
				pre[v]=cur;
			}
		}
	}
}
int main(int argc, char *argv[])
{
#ifdef CHAOS
	freopen("data.in","r",stdin);
#endif
	scanf("%d %d",&n,&m);
	int i;
	for(i=0;i<m;++i)
	{
		int a,b,w;
		scanf("%d %d %d",&a,&b,&w);
		adj[a].push_back(make_pair(b,w));
		adj[b].push_back(make_pair(a,w));
	}
	Dijkstra(1,n);
	if(dist[n]==INF) cout<<-1<<endl;
	else
	{
		vector<int> ans;
		while(n!=1)
		{
			ans.push_back(n);
			n=pre[n];
		}
		ans.push_back(1);
		n=ans.size();
		for(i=0;i<n;++i)
		{
			if(i) cout<<" ";
			cout<<ans[n-i-1];
		}
		cout<<endl;
	}
	return 0;
}
