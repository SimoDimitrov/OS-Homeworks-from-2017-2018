// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #3
// FILE NAME: swap
// FILE PURPOSE: swap
#include <stdio.h>
void swap(int *a, int *b);
int main()
{
   int a,b;
   scanf("%d",&a);
   scanf("%d",&b);

   swap(&a,&b);
   
    printf("%d\n",a);
    printf("%d",b);
}
void swap(int *a, int *b)
{
    int c;
    c=*a;
    *a=*b;
    *b=c;
}

