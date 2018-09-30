#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<errno.h>
#include<pwd.h>

int main(int argc,  char **argv){

struct stat s;
struct passwd *ps;
    for(int i=1;i<argc;i++){
        int m = stat(argv[i],&s);
        if(m!=0){
            perror("stat:");
            return 1;
        }
        ps = getpwuid(s.st_uid);
        printf("%s %s\n",argv[i],ps->pw_name);
    }
    return 0;
}
