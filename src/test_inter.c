#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MEM_SIZE 1024

int isBF(char *str) {
	if (str[strlen(str)-3] == '.' && str[strlen(str)-2] == 'b' && str[strlen(str)-1] == 'f')
		return 0;
	return 1;
}

int main (int argc, char *argv[]) {
	int fd, i;
	char c;
	/* Init memory. This could be changed to be dynamic or argument passed */
	unsigned char memory [MEM_SIZE];
	int m_pointer; // Memory pointer

	/* Check parameters */
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

	/* Init memory */ // Memory could be initialliced in a dynamic form with all 0's
	for (i = 0; i < MEM_SIZE; i++) {
		memory[i] = 0;
	}
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
		printf("MEM_INFO:\n\tm_pointer: %i\n\tvalue: %c\n", m_pointer, memory[m_pointer]);
	}
	
	return 0;
}

