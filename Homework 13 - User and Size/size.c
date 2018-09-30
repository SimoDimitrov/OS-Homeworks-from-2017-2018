#include<stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
int main(int argc,  char **argv){
struct stat s;
    for(int i=1;i<argc;i++){
    int z = stat(argv[i],&s);
    if(z!=0){
        perror("error");
        return 1;
    }
    printf("%s %ld\n",argv[i],s.st_size);
    }
    return 0;
}
