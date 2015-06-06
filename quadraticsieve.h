#ifndef QUADRATICSIEVE_H_INCLUDED
#define QUADRATICSIEVE_H_INCLUDED

#include "config.h"

struct factors;

/*
 * Run quadratic sieve to find factors to n. Return 1 if one or more factors
 * are found, 0 otherwise.
 *
 * It could be sensitive to small numbers depending on used thresh defined in
 * the configuration.
 */
int
quadratic_sieve(mpz_t n, struct factors *factors, ulong m);

#endif
