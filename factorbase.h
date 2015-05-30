#ifndef FACTORBASE_H_INCLUDED
#define FACTORBASE_H_INCLUDED

#include "config.h"

/*
 * Describes one prime number in a factor base.
 */
struct factor_prime {
	ulong	value;
	float	value_log;
	ulong	r[2];   /* quadratic residues */
};

/*
 * Describes a number of prime numbers that are smooth over a given size.
 */
struct factor_base {
	struct factor_prime	*primes;
	ulong			size;
};

struct factor_base *
factor_base_create(mpz_t n, mpz_t n_sqrt, ulong size);

void
factor_base_print(struct factor_base *fb);

void
factor_base_destroy(struct factor_base *fb);

#endif
