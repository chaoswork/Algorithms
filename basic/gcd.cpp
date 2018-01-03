/* greatest common divisor */
int gcd(int a,int b)
{
	return a?gcd(b%a,a):b;
}

