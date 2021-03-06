// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #1
// FILE NAME: bytes
// FILE PURPOSE: izkarva elements ot N byte do kraq

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int fd = open(argv[1], O_RDONLY); 
	int read_bytes, line = 0, last=0;
	int start_byte = atoi(argv[2]);
	int bytes = atoi(argv[3]);
	char buffer[1];
	
	off_t size = lseek(fd, start_byte, SEEK_SET);

	while(1)
	{
		read_bytes = read(fd, buffer, 1);

		if(read_bytes == 0) break;
		
		write(STDOUT_FILENO, buffer, 1);

		last++;
	
		if(last==bytes) break;                                                       
	}

	close(fd)
	return 0;
}