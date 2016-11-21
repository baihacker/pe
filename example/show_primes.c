const int maxp = 100000;
#include <pe>

int main()
{
	init_primes();
	
	int total = 0;
	for (int i = 0; i < pcnt; ++i)
	{
		const int v = plist[i];
		if (v > 10000) break;
		printf("%d ", v);
		++total;
	}
	puts("");
	
	printf("%d %d\n", pmpi[4], total);
	
	return 0;
}
