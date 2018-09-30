// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #4
// FILE NAME: void-swap
// FILE PURPOSE: void-swap

#include <stdio.h>
#include <stdlib.h>
void swap(void * a, void * b, size_t size);
int main()
{
}
void swap(void * a, void * b, size_t size)
{
	char *q=a, *w=b;
	size_t i;
	char *temp = malloc(size);

	for(i=0;i<size;i++)
	{
		*temp=q[i];
		q[i] = w[i];
		w[i] = *temp;
	}
}
