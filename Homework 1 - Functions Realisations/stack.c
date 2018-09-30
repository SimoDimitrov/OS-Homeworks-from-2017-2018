// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #6
// FILE NAME: stack
// FILE PURPOSE: stack
#include <stdio.h>
#include <stdlib.h>

struct simo
{
    size_t size;
    size_t capacity;
    int *elements;
}stack;

void stack_init(struct simo*);
void stack_destroy(struct simo*);

int stack_empty(struct simo*);
void stack_push(struct simo*, int);
int stack_top(struct simo*);
void stack_pop(struct simo*);

int main()
{
}

void stack_init(struct simo* stack)
{
	stack->size=0;
	stack->capacity=50;
	stack->elements = malloc(stack->capacity*sizeof(int));
}

void stack_destroy(struct simo* stack)
{
	stack->size=0;
	stack->capacity=0;
	free(stack->elements);
	stack->elements=NULL;
}

int stack_empty(struct simo* stack)
{
	return stack->size==0;
}

void stack_push(struct simo* stack, int a)
{
	stack->elements[stack->size++]=a;
}

int stack_top(struct  simo* stack){
	int b = --stack->size;
	return stack->elements[b];
}
void stack_pop(struct simo* stack){
	if(stack_empty)
	{
	return;
	}
	stack->size--;
}
