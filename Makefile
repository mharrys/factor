CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -O4
LIBS = -lm -lgmp

all: factor

factor: main.o primesieve.o
	$(CC) $(CFLAGS) $(LIBS) -o factor main.o primesieve.o

main.o: main.c
	$(CC) $(CFLAGS) $(LIBS) -c main.c

primesieve.o: primesieve.c
	$(CC) $(CFLAGS) $(LIBS) -c primesieve.c

.PHONY: clean

clean:
	rm factor
	rm *.o
