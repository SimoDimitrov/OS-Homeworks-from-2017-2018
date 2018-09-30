//--------------------------------------------
//NAME: Simeon Dimitrov
//CLASS: 11b
//NUMBER: 23
//PROBLEM: #1
//---------------------------------------------

#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <pthread.h>
 
int  gold;
pthread_mutex_t mutex; 
 
void* miner(void* number)
{
    int i;
    int chislo = (int)number;
    for (i = 0; i < 20; ++i)
    {
        pthread_mutex_lock(&mutex);
        gold += 10;
        printf("Miner %d gathered 10 gold\n",chislo+1);
        pthread_mutex_unlock(&mutex);       
        sleep(2);
    }
    return NULL;
}

void* trader(void* number)
{
    int i;
    int chislo = (int) number;

    for (i = 0; i < 20; ++i)
    {
        pthread_mutex_lock(&mutex);
        if(gold==0){
            printf("The warehouse is empty, cannot sell!\n");
        }else{    
            gold -= 10;   
            printf("Trader %d sold 10 gold\n",chislo+1);
        
        }
        pthread_mutex_unlock(&mutex);    
        sleep(2);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex,NULL);
    if(argc != 1){
        pthread_t threads_miner[atoi(argv[1])];
        pthread_t threads_trader[atoi(argv[2])];
        int i;
        for (i = 0; i < atoi(argv[1]); ++i)
        {    
            pthread_create(&threads_miner[i],NULL,miner,i);
        }
        for (i = 0; i < atoi(argv[2]); ++i)
        {
            pthread_create(&threads_trader[i],NULL,trader,i);
        }
     
        for (i = 0; i < atoi(argv[2]); ++i)
        {
            pthread_join(threads_trader[i], NULL);
        }
     
        for (i = 0; i < atoi(argv[1]); ++i)
        {
            pthread_join(threads_miner[i], NULL);
        }     
    }else{
        pthread_t threads_miner[1];
        pthread_t threads_trader[1];
        int i;
        for (i = 0; i < 1; ++i)
        {    
            pthread_create(&threads_miner[i],NULL,miner,i);
        }
        for (i = 0; i < 1; ++i)
        {
            pthread_create(&threads_trader[i],NULL,trader,i);
        }
     
         for (i = 0; i < 1; ++i)
        {
            pthread_join(threads_trader[i], NULL);
        }
     
        for (i = 0; i < 1; ++i)
        {
            pthread_join(threads_miner[i], NULL);
        }      
    }
    printf("Gold: %d\n",gold);
    pthread_mutex_destroy(&mutex);
    return 0;
}
