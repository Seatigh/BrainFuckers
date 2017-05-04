#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../lib/stack.h"

#define MEM_SIZE 10000
#define debug 0 // 0 off, 1 on
#define debug2 0

int fd, i, m_pointer, nest_level, action;
char c, in;
unsigned char memory [MEM_SIZE];
stack s;

int isBF(char *str) {
	if (str[strlen(str)-3] == '.' && str[strlen(str)-2] == 'b' && str[strlen(str)-1] == 'f')
		return 0;
	return 1;
}

void showMemory () {
	printf("Memory status:\n");
	/*for (i = 0; i < MEM_SIZE; i++) {
		printf("%d ", i);
	}
	printf("\n");*/
	for (i = 0; i < MEM_SIZE; i++) {
		printf("%c ", memory[i]);
	}
	/*char blanks [2*m_pointer];
	memset(blanks, ' ', 2*m_pointer);
	printf("\n%s*\n", blanks);*/
	printf("\n");
	for (i = 0; i < m_pointer; i++) {
		printf("  ");
	}
	printf("*\n");
}

/* Read file */
/*
 * >	pointer++
 * <	pointer--
 * +	*pointer++
 * -	*pointer--
 * .	print
 * ,	scan (i never used this
 * [	open while
 * ]	close while
 *
 */
void readBF () {
	int loop_pointer = 0; // Use a stack to the loop_pointers
	while(read(fd, &c, 1) == 1) {
		/* Conditions to parser 2 brainfuck */
		#if debug2
		printf("c: %c\n", c);
		#endif
		if (c == '>') {
			action++;
			#if debug
			printf("%d\tAction: >\t|\tmem pos: %d\n", action, (m_pointer + 1));
			#endif
			m_pointer = (m_pointer + 1) % MEM_SIZE;
		}
		else if (c == '<') {
			action++;
			#if debug
			printf("%d\tAction: <\t|\tmem pos: %d\n", action, (m_pointer - 1));
			#endif
			m_pointer = (m_pointer - 1) % MEM_SIZE;
		}
		else if (c == '+') {
			action++;
			#if debug
			printf("%d\tAction: +\t|\tmem[%d]: %d\n", action, m_pointer, (memory[m_pointer] + 1));
			#endif
			memory[m_pointer]++;
		}
		else if (c == '-') {
			action++;
			#if debug
			printf("%d\tAction: -\t|\tmem[%d]: %d\n", action, m_pointer, (memory[m_pointer] + 1));
			#endif
			memory[m_pointer]--;
		}
		else if (c == '.') {
			action++;
			#if debug
			printf("%d\tAction: .\t|\toutput: '%d' %c\n", action, (int) memory[m_pointer], memory[m_pointer]);
			#endif
			printf("%c", memory[m_pointer]);
		}
		else if (c == ',') {
			action++;
			#if debug
			printf("%d\tAction: ,\t|\tread stdin: %c (%d)\n", action, memory[m_pointer], (int) memory[m_pointer]);
			#endif
			if (scanf("%c", &in) < 1) {printf("Error: stdin read failed.\n"); return;}
			printf("input: %s\n", &in);
			memory[m_pointer] = in;
		}
		else if (c == '[') {
			action++;
			#if debug
			printf("%d\tAction: [\t|\tmem[%d] is '%d' ** Loop nesting level: %d\n", action, m_pointer, (int)  memory[m_pointer], nest_level);
			#endif
			if (memory[m_pointer] < 1) {
				#if debug
				printf("\t\t\t|\tMemory 0: jumping loop\n");
				#endif
				read(fd, &c, 1);
				while(c != ']')
					read(fd, &c, 1);
			} else {
				loop_pointer = lseek(fd, 0, SEEK_CUR) - 1;
				push(&s, loop_pointer);
				nest_level++;
				// simple: continue;
				#if debug
				printf("\t\t\t|\tPush %d\n", loop_pointer);
				#endif
			}
		} else if (c == ']') {
			action++;
			#if debug
			printf("%d\tAction: ]\t|\tmem[%d] is '%d'\n", action, m_pointer, (int) memory[m_pointer]);
			#endif
			int p = pop(&s);
			#if debug
			printf("\t\t\t|\tPop: %d\n", p);
			#endif
			if (memory[m_pointer] > 0) {
				lseek(fd, p, SEEK_SET);
				nest_level--;
				#if debug
				printf("\t\t\t|\tLooping back to %d\n", p);
				#endif
			}
			else {
				// simple: continue;	
			}
		}
		#if debug2
		showMemory();
		#endif
	}
}

int main (int argc, char *argv[]) {
	/********************
	 * Check parameters *
	 *******************/
	if (argc != 2) {
		fprintf(stderr, "Error: Invalid number of arguments.\nExe form:\n\t%s <.bf file>\n", argv[0]);
		return -1;
	}
	if (isBF(argv[1])) {
		fprintf(stderr, "Error: File must be a .bf file.\n");
		return -1;
	}
	/********************/
	
	/***********************
	 * Open and check file *
	 ***********************/
	fd = open(argv[1], O_RDONLY);
//	printf("fd: %i\n", fd);
	if (fd < 0) {
		fprintf(stderr, "Error: can't open file %s or it doesn't exits.\n", argv[1]);
		return -1;
	}

	printf("Interpret file: %s\n\n", argv[1]);

	/***************
	 * Init memory * // Memory could be initialliced in a dynamic way with all 0's
	 **************/
	memset(memory, 0, MEM_SIZE);
	m_pointer = 0;
	nest_level = 0;
	action = 0;
	s.pointer = 0;

	#if debug2
	showMemory();
	#endif
	readBF();
	printf("\n");	
	return 0;
}

