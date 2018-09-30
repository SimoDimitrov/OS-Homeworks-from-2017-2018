//------------------------------------------------------------------------
// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #1
// FILE NAME: starcraft.c 
// FILE PURPOSE: Program for digging minerals with multi pthreads
//------------------------------------------------------------------------

#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define sleep(p)

int bloks = 2; //number of mineral blocks
int marines = 0; //number of soldiers
int minersNum = 5; //number of miners
int centerMinerals = 0; //minerals in the command center
pthread_t miners_[1000]; //array for miners
pthread_mutex_t MutexMineralsCenter; //mutex for the command center
pthread_mutex_t mutex_soldiers; //mutex for the soldiers

int *mineralsOfBloks; //array for the mineral blocks
pthread_mutex_t *bloksMutex; //mutex for the array of blocks

void* miners(void* attr)
{
    int MinerNumber = (int*)attr; //which is the number of the miner
    int fullbloks[bloks]; //making array for all of the mineral blocks
    int breakCon = 0,j = 0,miner_resource = 0;
    for(int l = 0; l < bloks; l++)
    {
        fullbloks[l] = 1; //making all the blocks full of minerals
    }

    while(1)
    {
        for(int i = 0;i < bloks;i++) //check for full blocks
        {
            if(fullbloks[i] == 1)
            {
                breakCon++;
                break;
            }
        }
        if(breakCon > 0)
        {
            breakCon = 0;
        }else{
            return NULL;
        }
        
        sleep(3);

        if(pthread_mutex_trylock(&bloksMutex[j]) == 0) //entry and lock the mineral block if it`s free
        {
            if(mineralsOfBloks[j]== 0) //is the block empty?
            {
                fullbloks[j] = 0; //making the block empty in the array of blocks
                j++; //moving the counter to the next block
                j %= bloks; //do not allow to go to inexistant block 
                pthread_mutex_unlock(&bloksMutex[j]); //unlock the mutex for this block
                continue; //going to the next block (if there are free...)
            }
            if(mineralsOfBloks[j] > 8) //if the block has more than 8 minerals
            {
                mineralsOfBloks[j] -= 8; //removing 8 minerals from the block
                miner_resource += 8; //adding 8 minerals to the miner`s minerals            
            }
            else if(mineralsOfBloks[j] < 8 && mineralsOfBloks[j] != 0) //if the block has between 1 and 8 minerals
            {
                fullbloks[j] = 0;  //making the block empty in the array of blocks
                miner_resource += mineralsOfBloks[j]; //adding whatever is left to the miner`s minerals
                mineralsOfBloks[j] -= mineralsOfBloks[j]; //removing whatever is left from the block
            }
            printf("SCV %d is mining from mineral block %d\n",MinerNumber,j+1);
            pthread_mutex_unlock(&bloksMutex[j]); //unlock the mutex for this block
            printf("SCV %d is transporting minerals\n",MinerNumber);
            sleep(2);
            pthread_mutex_lock(&MutexMineralsCenter); //lock the mutex for the command center
            printf("SCV %d delivered minerals to the Command center\n",MinerNumber);
            if(miner_resource < 8) //if the miner has less than 8 minerals 
            {
                centerMinerals += miner_resource; //adding whatever is left to the command center
                miner_resource -= miner_resource; //remove whatever is left from the command center
            }else{
                centerMinerals += 8; //adding 8 minerals to the command center
                miner_resource -= 8; //removing 8 minerals from the miner`s minerals
            }
            pthread_mutex_unlock(&MutexMineralsCenter); //unlock the mutex for the command center
        }
        j++; //moving the counter to the next block
        j %= bloks; //do not allow to go to inexistant block 
    }
    return NULL;
}

void run()
{
    int p;
    char c = 0;
    int maxMinerals = bloks * 500;
    while(1)
    {
        scanf("%c",&c);
        switch(c)
        {
            case 's':
                pthread_mutex_lock(&MutexMineralsCenter); //lock the mutex for the command center
                if(centerMinerals < 50) //if there aren`t enough minerals for making a miner
                {
                    printf("Not enough minerals.\n");
                }else{
                    sleep(4); 
                    centerMinerals -= 50; //taking 50 minerals charge for the miner
                    printf("SCV good to go, sir.\n");                   
                    minersNum++; //adding one miner to the miners count
                    pthread_create(&miners_[minersNum],NULL,miners,minersNum); //creating a thread for the miner
                }
                pthread_mutex_unlock(&MutexMineralsCenter); //unlock the mutex for the command center
            break;
            case 'm':
                pthread_mutex_lock(&MutexMineralsCenter);
                if(centerMinerals < 50) //if there aren`t enough minerals for making a soldier
                {
                    printf("Not enough minerals.\n"); 
                }else{
                    sleep(1);  
                    pthread_mutex_lock(&mutex_soldiers); //lock the mutex for the soldiers
                    marines+=1; //adding one soldier to the soldiers count
                    pthread_mutex_unlock(&mutex_soldiers); //unlock the mutex for the soldiers
                    printf("You wanna piece of me, boy?\n");
                    centerMinerals-=50; //taking 50 minerals charge for the soldier
                }
                pthread_mutex_unlock(&MutexMineralsCenter); //unlock the mutex for the command centerr
            break;
        }
        pthread_mutex_lock(&mutex_soldiers); //lock the mutex for the soldiers
        if(marines == 20)
        {
            for (int i = 0; i < minersNum; ++i)
            {
                int z = pthread_join(miners_[i], NULL); //waiting the miners to end working
                if(z != 0)
                {
                    perror("error\n");
                    free(mineralsOfBloks);
                    free(bloksMutex);
                    return;
                }
            }
            printf("Map minerals %d, player minerals %d, SCVs %d, Marines %d\n",maxMinerals,centerMinerals,minersNum,marines);
            break;
        }
        pthread_mutex_unlock(&mutex_soldiers); //unlock the mutex for the soldiers
    }
    free(mineralsOfBloks);
    free(bloksMutex);
}

int main(int argc, char **argv)
{
    if(argc > 1) //If we have an argument for the mineral blocks making it to number of blocks
    {
        bloks = atoi(argv[1]);
    }

    int i = 0;
    centerMinerals = 0; //minerals in the command center
    mineralsOfBloks = malloc(bloks * sizeof(int)); //making the array for the mineral blocks with the size of count of the blocks
    bloksMutex = malloc(bloks * sizeof(pthread_mutex_t)); //array from mutexes for every mineral block
    pthread_mutex_init(&MutexMineralsCenter,NULL); //mutex for the command center
    pthread_mutex_init(&mutex_soldiers,NULL); //mutex for the soldiers

    for(i = 0; i < bloks; i++) //adding 500 minerals in every mineral block
    {
        mineralsOfBloks[i] = 0;
        pthread_mutex_init(&bloksMutex[i],NULL); //mutex for the mineral blok
        mineralsOfBloks[i] += 500;
    } 
    for (i = 0; i < 5; ++i) //creating a thread for every miner
    {   
        int p = pthread_create(&miners_[i],NULL,miners,i + 1);
        if(p != 0){
            perror("error");
            free(mineralsOfBloks);
            free(bloksMutex);
            return -1;
        }
    }
    run();
    return 0;
}