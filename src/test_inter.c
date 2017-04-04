#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MEM_SIZE 15
#define STACK_SIZE 10
#define debug 1 // 0 off, 1 on

int fd, i, m_pointer, loop_counter;
char c;
unsigned char memory [MEM_SIZE];
unsigned int stack [STACK_SIZE] = {0};

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
		printf("%d ", (int) memory[i]);
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
		#if debug
		printf("c: %c\n", c);
		#endif
		if (c == '>')
			m_pointer = (m_pointer + 1) % MEM_SIZE;
		else if (c == '<')
			m_pointer = (m_pointer - 1) % MEM_SIZE;
		else if (c == '+')
			memory[m_pointer]++;
		else if (c == '-')
			memory[m_pointer]--;
		else if (c == '.')
			printf("%c", memory[m_pointer]);
		else if (c == '[') {
			loop_pointer = lseek(fd, 0, SEEK_CUR) - 1;
			if (memory[m_pointer] < 1) {
				printf("Memory 0: jumping loop\n");
				read(fd, &c, 1);
				while(c != ']')
					read(fd, &c, 1);
			} else
				continue;
		} else if (c == ']') {
			// 2 possible solutions. Uncomment code to get the complex one
			/*if (memory[m_pointer] > 0)*/
				lseek(fd, loop_pointer, SEEK_SET);
			/*else
				continue;*/
		}
		#if debug
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
	printf("fd: %i\n", fd);
	if (fd < 0) {
		fprintf(stderr, "Error: can't open file %s or it doesn't exits.\n", argv[1]);
		return -1;
	}

	printf("Interpret file: %s\n", argv[1]);

	/***************
	 * Init memory * // Memory could be initialliced in a dynamic way with all 0's
	 **************/
	memset(memory, 0, MEM_SIZE);
	m_pointer = 0;
	loop_counter = 0;

	#if debug
	showMemory();
	#endif
	readBF();
	printf("\n");	
	return 0;
}

