// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #1
// FILE NAME: shell.c
// FILE PURPOSE: Program which implements system shell
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char **parse_cmdline(const char *cmdlin);

int main()
{   
	while(1) //command loop
	{ 
	    write(1,"$ ",2); //puts dollar sign on every line
	    ssize_t size=0;
	    char *input;	    
	    getline(&input,&size,stdin); //gets all elements from the input
	    if(feof(stdin)) //if we simulate end of input with Ctrl+D it ends the program
		{
			free(input);
			break;
		}
	    char **array = parse_cmdline(input); //seperating the command line to tokens
	    free(input); //free the memory for the line

        pid_t pid = fork(); //making child process to execute the current command

        if(pid < 0)
	    {
            perror("fork");
        }

        else if(pid == 0) //successful fork
        {
            int p = execv(array[0],array); //executing the current command
        	if(p < 0) 
			{
	        perror(array[0]);
	        int z=0;
                char *ptr = array[0]; 
                while(ptr!=NULL) //free the memory for every element of the array with the command
				{
                	free(ptr);
                	z++;
                	ptr = array[z];
                }
                free(array);
	            exit(0);
			}
            exit(0);	
	}else{
		waitpid(pid, 0 ,0); //parent process waits the child process
	}     
        int z=0;
        char *ptr = array[0]; 
        while(ptr!=NULL)
		{
        	free(ptr); //free the memory for every element of the array with the command
        	z++;
        	ptr = array[z];
        }
        free(array);
	}
return 0;
}

char **parse_cmdline(const char *cmdlin) //function which parses the input line
{    
    int str_sym=0, last_sym=0, z=0, sto=100000000, word=0, current_symbol, word_symbol;
    char *str = malloc((strlen(cmdlin)+1) * sizeof(char)); //allocating memory for the string with 
                                                           //whole input line
    strcpy(str,cmdlin); //copy the input line in the string
    char **new_str=malloc((word+sto)*sizeof(char*)); //allocating memory for the double-size array which
                                                     // will be returned
	size_t len = strlen(str);
    for(;str_sym<len;str_sym++) //loop to explore the whole input line
    {
        if(str[str_sym]==' ' || str[str_sym]=='\n') //we have passed one word or we are on the last
        {   
            if(last_sym!=0) last_sym++;
            new_str[word] = malloc((str_sym - last_sym)*sizeof(char)); //set memory for the word(command)
            
            for(current_symbol=last_sym,word_symbol=0;current_symbol<str_sym;current_symbol++,word_symbol++) // loop to explore the symbols of the current word(command)
            {           
                new_str[word][word_symbol]=str[current_symbol]; //make the word into double-sized array
            }
	    new_str[word][word_symbol] = '\0'; //to be sure that we have only the word
            z++; //one more word --> more memory
            last_sym=str_sym; //moving the start of the next word to the end of the previous one
            word++; // number of words we have passed is now bigger with 1
	    if(z==sto) //if we exceed the memory and we need more
	    {
		z=0; //memory to zero for the second lap
            	new_str = realloc(new_str,(word+sto)*sizeof(char*)); // get one more big piece memory
	    }
       }
    }
    free(str); //free the memory for the string with input commands
    new_str[word] = NULL; //one more surable end
    return new_str; //the result - two-sized array with the commands
}
