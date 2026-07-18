long long factorial(int x)
{
	int z = 1;
	int i = 0;
	for (i = 1; i <= x; i++)
	{
		z *= i;
	}
	return z;
}
long long factorial_sum(int n)
{
	int i = 0;
	long long knx = 0;
	for (i = 1; i <= n; i++) 
	{
		knx += factorial(i);
	}
	return knx;
}
long long combination_sum(int n)
{
	int i = 0;
	int j = 0;
	long long knx = 0;
	for (i = 1; i <= n; i++)
	{
		knx += (factorial(n)/factorial(n-i))/ factorial(i);
	}
	return knx;
}