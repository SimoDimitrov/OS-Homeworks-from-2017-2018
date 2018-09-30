// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #5
// FILE NAME: strdup
// FILE PURPOSE: strdup

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *strdup(const char *);
int main()
{

}
char *strdup(const char niz[])
{
    int i;
    char *d = malloc (sizeof(niz));
    for(i=0;i<strlen(niz);i++)
    {
        d[i]=niz[i];
    }
    d[strlen(niz)]='\0';
    return d;
}
