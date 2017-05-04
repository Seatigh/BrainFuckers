#ifndef STACK
#define STACK

#define SIZE 1024

typedef struct stack {
	unsigned int stack [SIZE];
	int pointer;
} stack;

int push (stack *s, unsigned char c);

unsigned char pop (stack *s);

void top (stack *s);

int isEmpty (stack *s);

#endif
