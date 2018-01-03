#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int n,k;
typedef struct node
{
	int mn; 	//min Value
	int mx; 	//max Value
	int l;  	//left position
	int r;  	//right position
}node;
const int maxn=100010;
node seg[maxn*4];
int num[maxn];	//store the number list

/* build the Segment Tree */
void buildSeg(int l,int r,int idx)
{
	/* leaf node */
	if(l==r)
	{
		seg[idx].mn=num[l];
		seg[idx].mx=num[l];
		seg[idx].l=l;
		seg[idx].r=r;
		return;
	}
	/* divide and conquer */
	int mid=l+(r-l)/2;			//in case l and r is too large to add overflow
	buildSeg(l,mid,idx*2);		//idx has two child,2*idx
	buildSeg(mid+1,r,idx*2+1);	//and 2*idx+1

	seg[idx].mx=max(seg[idx*2].mx,seg[idx*2+1].mx);
	seg[idx].mn=min(seg[idx*2].mn,seg[idx*2+1].mn);
	seg[idx].l=l;
	seg[idx].r=r;
	return;
}
/* find the max value and the min Value from l to r in num */
void findSeg(int l,int r,int gl,int gr,int idx,int& mn,int& mx)
{
	if(l==seg[idx].l && r==seg[idx].r)
	{
		mx=seg[idx].mx;
		mn=seg[idx].mn;
		return;
	}
	int tmn,tmx;
	int mid=gl+(gr-gl)/2;
	if(r<=mid)
	{
		findSeg(l,r,gl,mid,idx*2,tmn,tmx);
		mn=tmn;
		mx=tmx;
	}
	else if(l>mid)
	{
		findSeg(l,r,mid+1,gr,idx*2+1,tmn,tmx);
		mn=tmn;
		mx=tmx;
	}
	else 
	{
		findSeg(l,mid,gl,mid,idx*2,tmn,tmx);
		mn=tmn;
		mx=tmx;
		findSeg(mid+1,r,mid+1,gr,idx*2+1,tmn,tmx);
		mn=min(tmn,mn);
		mx=max(tmx,mx);
	}
	return;
}
