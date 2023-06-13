# Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# creation fichier et executable
program: cx17.7.o indexation.o 
	$(CC) $(CFLAGS) cx17.7.o indexation.o -o cx17.7

cx17.7.o: cx17.7.c indexation.h
	$(CC) $(CFLAGS) -c cx17.7.c
	
indexation.o: indexation.c indexation.h
	$(CC) $(CFLAGS) -c indexation.c

#nettoyage	
clean:
	rm -f program *-o cx17.7.o indexation.o


