//------------------------------------------------------------------------
// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #1
// FILE NAME: simo.c 
// FILE PURPOSE: Program which calculate the number of all prime numbers till number
//------------------------------------------------------------------------
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
 
void *primesfun(void* Number)
{
    int n;
    n = (int)Number;
    int num_of_primes=0;
    int help = 1,j=2,p=3;
    while(j<n)
    {       
        for(p=2;p<j;p++){
            if(j%p==0)
	    {
                help=0;
                break;
            }  
        }
        if(help)
	       {
            num_of_primes++;
        }
        help = 1;
        j++;
    }
    printf("Number of primes for N=%d is %d\n",n,num_of_primes);
    fflush(stdout);
}

int main()
{
    pthread_t thread[60];
    char arr[150];
    int n, broi_nishki=0;
    while(1)
    {
        scanf("%s",arr);
        scanf("%d",&n);
        int len = strlen(arr);
        if(len==1 && arr[0]=='e')
        {
            break;
        }else if(len==1 && arr[0]=='p')
        {
            printf("Prime calculation started for N=%d\n", n);
            pthread_create(&thread[broi_nishki],NULL,primesfun,(void*)n);
            broi_nishki++;
        }else
        {
            printf("Supported commands:\n p N - Starts a new calculation for the number of primes from 1 to N\n e - Waits for all calculations to finish and exits\n");
        }
    }
    for(int num_of_threads = 0; num_of_threads < broi_nishki; num_of_threads++)
    {
        pthread_join(thread[num_of_threads],NULL);
    }
    return 0;
}
