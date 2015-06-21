CC := gcc
CFLAGS := -std=c99 -pedantic -Wall -O4
LIBS := -lm -lgmp -lmpfr
C_FILES := $(wildcard *.c)
OBJ_FILES := $(C_FILES:.c=.o)

factor: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

obj/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f factor
	rm -f *.o
