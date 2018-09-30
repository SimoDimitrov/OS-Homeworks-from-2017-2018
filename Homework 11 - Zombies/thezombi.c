// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #1
// FILE NAME: thezombi.c
// FILE PURPOSE: Game with zombies
#include "ui.h"
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
int health = 100;
int gold = 100;
int zombitata = 1;
int soldiers = 0;

pthread_mutex_t miners; 
pthread_mutex_t soldiers_mutex; 
pthread_mutex_t mutex; 

void* Zmb()
{
	while(1){
		for(int i = 5; i > 0;i--){
			pthread_mutex_lock(&soldiers_mutex);   
	    	print_zombies(i, zombitata);
	    	pthread_mutex_unlock(&soldiers_mutex);
	    	sleep(1);
	    }
	    pthread_mutex_lock(&soldiers_mutex);   
		if (zombitata > soldiers){
			health  -= (zombitata - soldiers);
	    	print_fail("Zombie attack succeded ;(!");
	    	print_health(health);
	    	if(health <= 0){
	    		game_end(zombitata);
	    	}
		}else{
			print_succ("Zombie attack deflected! :)");
		}	   
		zombitata *= 2;
		pthread_mutex_unlock(&soldiers_mutex);
	}
}

void* Mnr()
{
    while(1)
    {
        pthread_mutex_lock(&miners);
        gold += 10;   
        print_gold(gold);     
        pthread_mutex_unlock(&miners);
        sleep(1);
    }
}

int main() {
	init();
	print_gold(gold);
	print_soldiers(soldiers);
	print_health(health);
	pthread_mutex_init(&mutex,NULL);
	pthread_t miner;

	pthread_t Zombi;
	pthread_create(&Zombi,NULL,Zmb,NULL);

	while(1) {
		int ch = get_input();
		switch(ch) {
			case 'q':
				pthread_mutex_lock(&mutex);
				game_end(1);
				pthread_mutex_unlock(&mutex);
				break;
			case 's':
				pthread_mutex_lock(&soldiers_mutex);
				if(gold >= 10){
        			gold -= 10;   
					print_gold(gold);
					soldiers++;
					print_soldiers(soldiers);
					print_msg("Soldier created!");
				}else{
					print_fail("Not enough gold!");
				}
   				pthread_mutex_unlock(&soldiers_mutex);
				break;
			case 'm':
				pthread_mutex_lock(&miners);
				if(gold >= 100){
        			gold -= 100;   
					pthread_create(&miner,NULL,Mnr,NULL);
					print_gold(gold);
					print_msg("Miner created!");
				}else{
					print_fail("Not enough gold!");
				}
				pthread_mutex_unlock(&miners);
				break;
			case 'x':
				pthread_mutex_lock(&soldiers_mutex);
				if(gold >= 100){
        			gold -= 100;   
					soldiers += 10;
					print_gold(gold);
					print_soldiers(soldiers);
					print_msg("10 x soldiers created!");
				}else{
					print_fail("Not enough gold!");
				}
				pthread_mutex_unlock(&soldiers_mutex);
				break;
		}
	}
}
