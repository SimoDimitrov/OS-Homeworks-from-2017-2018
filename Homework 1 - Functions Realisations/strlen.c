#include <stdio.h>
int strlen(const char*);
int main()
{
    int result;
    char pesho[500];
    printf("Type a string:\n");
    fgets(pesho,500,stdin);

    result=strlen(pesho);
    printf("%d",result);
}
int strlen(const char pesho[])
{
    int i;
    for(i=0;pesho[i]!='\0';i++);
    return i;
}
