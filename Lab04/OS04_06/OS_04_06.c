#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	for (int i = 1; i < 40000000000; i++)
	{
		printf("%d\n",i);
	}
	exit(0);
}
