CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -O4
LIBS = -lm -lgmp

all: factor

factor: main.o factors.o primesieve.o trialdivision.o matrix.o sqrtm.o factorbase.o
	$(CC) $(CFLAGS) $(LIBS) -o factor main.o factors.o primesieve.o trialdivision.o matrix.o sqrtm.o factorbase.o

main.o: main.c
	$(CC) $(CFLAGS) $(LIBS) -c main.c

factors.o: factors.c
	$(CC) $(CFLAGS) $(LIBS) -c factors.c

primesieve.o: primesieve.c
	$(CC) $(CFLAGS) $(LIBS) -c primesieve.c

trialdivision.o: trialdivision.c
	$(CC) $(CFLAGS) $(LIBS) -c trialdivision.c

matrix.o: matrix.c
	$(CC) $(CFLAGS) $(LIBS) -c matrix.c

sqrtm.o: sqrtm.c
	$(CC) $(CFLAGS) $(LIBS) -c sqrtm.c

factorbase.o: factorbase.c
	$(CC) $(CFLAGS) $(LIBS) -c factorbase.c

.PHONY: clean

clean:
	rm factor
	rm *.o
