# Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Wpedantic
LDFLAGS = -L. -lindexer
LIBRARY_PATH = /usr/lib

# main
all: cx17.8

# creation fichier et executable
libindexer.so: indexer.o
	$(CC) -shared -o libindexer.so indexer.o

indexer.o: indexer.c indexer.h
	$(CC) -c -fPIC $(CFLAGS) indexer.c -o indexer.o

cx17.8: cx17.8.c libindexer.so
	$(CC) $(CFLAGS) -o cx17.8 cx17.8.c $(LDFLAGS)

#installation librairie
install: libindexer.so
	sudo cp libindexer.so $(LIBRARY_PATH)

#nettoyage
clean:
	rm -f indexer.o libindexer.so cx17.8
