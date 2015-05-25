#ifndef TRIALDIVISION_H_INCLUDED
#define TRIALDIVISION_H_INCLUDED

#include "config.h"

struct factors;
struct prime_sieve;

/*
 * Perform trial division on n using prime sieve ps. Return 1 if one or more
 * factors were found, 0 otherwise.
 */
int
trial_division(mpz_t n, struct factors *factors, struct prime_sieve *ps);

#endif
