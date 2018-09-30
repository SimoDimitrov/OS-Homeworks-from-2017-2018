// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #1
// FILE NAME: simoChat.c
// FILE PURPOSE: Chat program in the terminal(s)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10

int main(int argc, char **argv){

    int fd,fd2;
    char buffer[SIZE];
    ssize_t rbytes;

    int chat = mkfifo("chat1",0777);
    int chat1 = mkfifo("chat",0777);

    fd = open("chat", O_RDWR);
    if(fd<0){
        perror("open first fifo");
    }

    fd2 = open("chat1", O_RDWR);
    if(fd2<0){
        perror("open second fifo");
    }

    pid_t child = fork();
    
    if(child < 0){
    	perror("fork");
    }else if(child == 0){
        if(strcmp("First",argv[1])==0)
        { 
        	while( (rbytes = read(fd, buffer, SIZE)) != 0 )
        	{
                write(STDOUT_FILENO, buffer, rbytes);         
        	}
        }else if(strcmp("Second",argv[1])==0){
            while( (rbytes = read(fd2, buffer, SIZE)) != 0 )
            {
                write(STDOUT_FILENO, buffer, rbytes);
            }
        }
    	exit(0);
    }else{
        if(strcmp("First",argv[1])==0){
    		while( (rbytes = read(STDIN_FILENO, buffer, SIZE)) != 0 )
    		{
                write(fd2, buffer, rbytes);
    		}
        }else if(strcmp("Second",argv[1])==0){
            while( (rbytes = read(STDIN_FILENO, buffer, SIZE)) != 0 )
            { 
                write(fd, buffer, rbytes);
            }   
        }	

    }
    close(fd2);
    close(fd);
	return 0;
}
