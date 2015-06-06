#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <gmp.h>
#include <mpfr.h>

typedef unsigned long int ulong;

#define ULONG_BITS  64
#define ULONG_BYTES 8

#define INTEGER_LIMIT		255
#define MILLERRABIN_REPEATS	20
#define TRIALDIVISION_LIMIT	2500000
#define QUADRATIC_SIEVE_SIZE	80000
#define QUADRATIC_SIEVE_THRESH	5.0	/* close to zero */
#define QUADRATIC_SIEVE_MAGIC	1.0	/* badly chosen and QS could eat up all your RAM */

#define MIN(A, B) (A < B) ? A : B

#endif
