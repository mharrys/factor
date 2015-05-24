#ifndef PRIMESIEVE_H_INCLUDED
#define PRIMESIEVE_H_INCLUDED

#include "config.h"

struct prime_sieve {
	ulong	size;
	ulong	*primes;
};

/*
 * Create a prime number sieve using Erathosthenes algorithm.
 */
struct prime_sieve *
prime_sieve_create(ulong n);

void
prime_sieve_destroy(struct prime_sieve *ps);

#endif
