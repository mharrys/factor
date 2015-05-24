#ifndef FACTORS_H_INCLUDED
#define FACTORS_H_INCLUDED

#include "type.h"

struct factors {
	ulong	size;
	ulong	capacity;   /* allocated array size */
	mpz_t	*factors;
};

struct factors *
factors_create();

void
factors_destroy(struct factors *f);

void
factors_push(struct factors *f, mpz_t factor);

void
factors_pop(struct factors *f, mpz_t factor);

/*
 * Remove first composite from array of factors and copy factor to parameter.
 * Return 1 if composite was found, 0 otherwise.
 */
int
factors_remove_composite(struct factors *f, mpz_t factor);

void
factors_peek_front(struct factors *f, mpz_t front);

void
factors_peek_end(struct factors *f, mpz_t end);

void
factors_sort(struct factors *f);

#endif
