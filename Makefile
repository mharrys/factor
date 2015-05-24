CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -O4
LIBS = -lm -lgmp

all: factor

factor: main.o
	$(CC) $(CFLAGS) $(LIBS) -o factor main.o

main.o: main.c
	$(CC) $(CFLAGS) $(LIBS) -c main.c

.PHONY: clean

clean:
	rm factor
	rm *.o
