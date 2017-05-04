#include "../lib/stack.h"
#include <stdio.h>
#include <stdlib.h>

int push (stack *s, unsigned char c) {
	if (s->pointer < SIZE) {
		s->stack[s->pointer] = c;
//		printf("%c\t%d\n", s->stack[s->pointer], s->pointer);
		s->pointer++;
//		printf("pointer: %d\n", s->pointer);
		return 0;
	} else {
		printf("Error - full stack: cannot push more elements.\n");
		return -1;
	}
}

unsigned char pop (stack *s) {
	if (!isEmpty(s)) {
		s->pointer--;
		char c = s->stack[s->pointer];
		s->stack[s->pointer] = 0;
		return c;
	} else {
		printf("Error - empty stack: cannot pop any element.\n");
		return 0;
	}
}

void top (stack *s) {
	if (isEmpty(s) == 0)
		printf("%d\n", (int) s->stack[s->pointer-1]);
	else
		printf("Error - empty stack: there is no element to show.\n");
}

int isEmpty (stack *s) {
//	printf("isEmpty pointer: %d\n", s->pointer);
	if (s->pointer > 0)
		return 0;
	else
		return 1;
}
