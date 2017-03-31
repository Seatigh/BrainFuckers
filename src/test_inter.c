#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MEM_SIZE 15

int i;

int isBF(char *str) {
	if (str[strlen(str)-3] == '.' && str[strlen(str)-2] == 'b' && str[strlen(str)-1] == 'f')
		return 0;
	return 1;
}

void showMemory (unsigned char * mem, int p) {
	printf("Memory status:\n");
	/*for (i = 0; i < MEM_SIZE; i++) {
		printf("%d ", i);
	}
	printf("\n");*/
	for (i = 0; i < MEM_SIZE; i++) {
		printf("%c ", mem[i]);
	}
	/*char blanks [2*p];
	memset(blanks, ' ', 2*p);
	printf("\n%s*\n", blanks);*/
	printf("\n");
	for (i = 0; i < p; i++) {
		printf("  ");
	}
	printf("*\n");
}

int main (int argc, char *argv[]) {
	int fd;
	char c;
	/* Init memory. This could be changed to be dynamic or argument passed */
	unsigned char memory [MEM_SIZE];
	int m_pointer; // Memory pointer

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

	/* Open and check file */
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
	memset(memory, 'a', MEM_SIZE);
	m_pointer = 0;

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
	//showMemory(memory, m_pointer);
	while(read(fd, &c, 1) == 1) {
		/* Conditions to parser 2 brainfuck */
		if (c == '>')
			m_pointer = (m_pointer + 1) % MEM_SIZE;
		else if (c == '<')
			m_pointer = (m_pointer - 1) % MEM_SIZE;
		else if (c == '+')
			memory[m_pointer] = (memory[m_pointer] + 1) % 255;
		else if (c == '-')
			memory[m_pointer] = (memory[m_pointer] - 1) % 255;
		else if (c == '.')
			printf("%c", memory[m_pointer]);
		else if (c == '[') {
			// start while loop
			if (memory[m_pointer] < 1) {
				read(fd, &c, 1);
				while (c != ']') {
					read (fd, &c, 1);
				}
			}
		} else if (c == ']') {
			// end while loop
			if (memory[m_pointer] > 0) {
				// move pinter back to '['
			}
		}
		//showMemory(memory, m_pointer);
	}
	printf("\n");	
	return 0;
}

