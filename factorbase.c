#include "factorbase.h"

#include "primesieve.h"
#include "sqrtm.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct factor_base *
factor_base_create(mpz_t n, mpz_t n_sqrt, ulong size)
{
	mpz_t t, r, r1, r2;
	mpz_inits(t, r, r1, r2, NULL);

	struct prime_sieve *ps;
	ps = prime_sieve_create(size);

	struct factor_base *fb;
	fb = malloc(sizeof(*fb));
	/*
	 * pre-allocate since we know that the factor base is less than or equal
	 * to the smoothness bound
	 */
	fb->primes = malloc(size * sizeof(*fb->primes));
	fb->size = 0;

	for (ulong i = 0; i < ps->size; i++) {
		mpz_set_ui(t, ps->primes[i]);
		if (mpz_legendre(n, t) == 1) {
			struct factor_prime *prime = fb->primes + fb->size;
			prime->value = ps->primes[i];
			prime->value_log = log(ps->primes[i]);

			/* find the quadratic residue r^2 = n mod p */
			sqrtm(r, n, t);
			mpz_set(r1, r);
			mpz_neg(r2, r1);

			/* extract the solution t = (+/-)r_i - sqrt(n) mod p */
			mpz_sub(t, r1, n_sqrt);
			mpz_mod_ui(t, t, ps->primes[i]);
			prime->r[0] = mpz_get_ui(t);

			mpz_sub(t, r2, n_sqrt);
			mpz_mod_ui(t, t, ps->primes[i]);
			prime->r[1] = mpz_get_ui(t);

			fb->size++;
		}
	}

	/* adjust factor base to only use enough memory for the added primes */
	fb->primes = realloc(fb->primes, fb->size * sizeof(*fb->primes));

	mpz_clears(t, r, r1, r2, NULL);
	prime_sieve_destroy(ps);

	return fb;
}

void
factor_base_destroy(struct factor_base *fb)
{
	free(fb->primes);
	free(fb);
}

void
factor_base_print(struct factor_base *fb)
{
	printf("factor base: ");
	for (ulong i = 0; i < fb->size; i++) {
		printf("%lu ", fb->primes[i].value);
	}
	printf("\n");

	printf("factor base roots: ");
	for (ulong i = 0; i < fb->size; i++) {
		printf("(%lu %lu)", fb->primes[i].r[0], fb->primes[i].r[1]);
	}
	printf("\n");
}
