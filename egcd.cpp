/* ax+by=1 */
int egcd(int a,int b,int& x,int& y)
{
	if(!b){x=1;y=0;return a;}
	int d=egcd(b,a%b,y,x);
	y-=a/b*x;
	return d;
}
