// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #1
// FILE NAME: tail.c
// FILE PURPOSE: printing last 10 lines from text file or some text files and adding headers
// if they are more than 1 for better recognition

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc ,char **argv)
{
	int count = 0, check_break = 0, size;
	char strelka_lqvo[4] = {"==> "};
    char strelka_dqsno[5] = {" <==\n"};
    char buffer[1];

	for(int i=1;i<argc;i++) //checking all arguments from stdin 
	{
        if(argc!=2) //printing the headers if there are more than 1 argument (except ./)
		{
            write(STDOUT_FILENO, strelka_lqvo, 4);
            write(STDOUT_FILENO, argv[i], strlen(argv[i]));
            write(STDOUT_FILENO, strelka_dqsno, 5);
        }
	
		int fd = open( argv[i],O_RDONLY); 
		if(fd < 0) //checks for open errors
		{
            write(STDOUT_FILENO,"tail: cannot open '",19);
            write(STDOUT_FILENO, argv[i], strlen(argv[i]));
            write(STDOUT_FILENO,"' for reading: ",15);
            perror("");
			
			if(i!=argc-1) //not printing new line if we are on the last file
			{
                write(STDOUT_FILENO, "\n", 1);
            }
            
			continue;
      	}

		lseek(fd,0,SEEK_SET);
		count = 0;
		for(;;) //checking if the file is small than 10 lines
		{
			size = read(fd, buffer, 1); 
            if(size == 0 && count <= 10)
			{
				lseek(fd,0,SEEK_SET);
				for(;;) //printing the file if it`s smaller than 10 lines
				{
                    if(read(fd, buffer, 1) == 0){break;}
                    write(STDOUT_FILENO, buffer, 1);
			    }
				check_break = 1;
				break;
			}
			
            if(count == 10) break; 
            if(buffer[0]=='\n') //checking for new line
			{
                count++;
            }
			
		}
		
		if(check_break == 1) //cheking if small file is printed and if it is - skips printing for big files
		{
			if(i!=argc-1)
			{
				write(STDOUT_FILENO, "\n", 1);
			}
			
			continue;
		}
		
		lseek(fd,-1,SEEK_END);
		count = 0;

		while(1) //sets the current byte to be the first of the last 10 lines
		{
			read(fd, buffer, 1);
			if(count == 11) break;
			else if(buffer[0]=='\n')
			{
				count++;
			}

			if(count==11)
			{
			lseek(fd,-1,SEEK_CUR);
			}else{
			lseek(fd,-2,SEEK_CUR);
			}
		}

		for(;;) //printing the last 10 lines
		{
            if(read(fd, buffer, 1) == 0) break;
			write(STDOUT_FILENO, buffer, 1);
		}
		
		if(i!=argc-1)
		{
			write(STDOUT_FILENO, "\n", 1);
		}
			
		if(close(fd) < 0) //checks for close errors
		{
			write(STDOUT_FILENO,"tail: error reading '",21);
			write(STDOUT_FILENO, argv[i], strlen(argv[i]));
			write(STDOUT_FILENO,"': ",3);
			perror("");
			return 2;
		}
    }
}
