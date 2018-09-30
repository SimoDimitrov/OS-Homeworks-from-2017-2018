// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #2
// FILE NAME: strcat
// FILE PURPOSE: strcat

#include <stdio.h>
char* strcat(char* destination, const char* source);
int main()
{
    char destination[100];
    const char source[100];
    printf("Insert string N1: ");
    scanf("%s",&destination);
    printf("Insert string N2: ");
    scanf("%s",&source);

    char * result = destination[0];
    int o;
    result=strcat(destination,source);
    for(o=0;destination[o]!='\0';o++)
    {
        printf("%c",destination[o]);
    }
}
char* strcat(char* destination, const char* source)
{
    int i,p;
    for(i=0;destination[i]!='\0';i++);
    for(p=0;source[p]!='\0';p++,i++)
    {
        destination[i]=source[p];
    }
    return *destination;
}
