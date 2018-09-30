// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #1
// FILE NAME: head
// FILE PURPOSE: izkarva first 10 lines of text file

#include <stdio.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 

int main() 
{ 
    int fd = open("a.txt",O_RDONLY);    
    int bytes,line = 0; 
    char buffer[1];
     
    while(1) 
    {         
        bytes = read(fd,buffer,1);     

        if(bytes == 0) 
        { 
            break; 
        } 

        if(line == 10) 
        { 
            break; 
        } 

        else if(buffer[0]=='\n') 
        { 
            line++; 
        } 

        write(STDOUT_FILENO, buffer, 1); 
    } 
        close(fd); 
    return 0; 

}
