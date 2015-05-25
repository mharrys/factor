CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -O4
LIBS = -lm -lgmp

all: factor

factor: main.o factors.o primesieve.o trialdivision.o
	$(CC) $(CFLAGS) $(LIBS) -o factor main.o factors.o primesieve.o trialdivision.o

main.o: main.c
	$(CC) $(CFLAGS) $(LIBS) -c main.c

factors.o: factors.c
	$(CC) $(CFLAGS) $(LIBS) -c factors.c

primesieve.o: primesieve.c
	$(CC) $(CFLAGS) $(LIBS) -c primesieve.c

trialdivision.o: trialdivision.c
	$(CC) $(CFLAGS) $(LIBS) -c trialdivision.c


.PHONY: clean

clean:
	rm factor
	rm *.o
