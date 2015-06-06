#include "quadraticsieve.h"

#include "factors.h"
#include "factorbase.h"
#include "matrix.h"
#include "primesieve.h"
#include "smooth.h"
#include "trialdivision.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct quadratic_sieve {
	int	*pivots;
	float	*sieve;
	float	thresh;
	float	startup;

	ulong	b, m;
	ulong	min_sieve;
	ulong	max_sieve;
	ulong	num_smooth;
	ulong	max_smooth;

	mpz_t	n_sqrt;
	mpz_t	*smooth_relations;
	mpz_t	*least_p_residues;

	struct factor_base	*fb;
	struct matrix		*A;
};

static void
ceil_sqrt(mpz_t n, mpz_t n_sqrt)
{
	mpfr_t q;
	mpfr_init(q);

	mpfr_set_z(q, n, MPFR_RNDU);
	mpfr_sqrt(q, q, MPFR_RNDU);
	mpfr_get_z(n_sqrt, q, MPFR_RNDU);

	mpfr_clear(q);
	mpfr_free_cache();
}

/*
 * Cheap computation of the highest value in intervall to save time.
 */
static void
sieve_compute_startup(struct quadratic_sieve *qs, mpz_t n)
{
	mpz_t t;
	mpfr_t q;

	mpz_init(t);
	mpfr_init(q);

	/* using quadratic polynomial: Q(x) = (sqrt(n) + x)^2 - n */
	mpz_set_ui(t, (qs->m - 1));

	mpz_add(t, t, qs->n_sqrt);
	mpz_mul(t, t, t);
	mpz_sub(t, t, n);
	mpz_abs(t, t);

	/*
	 * use log(Q(x)) and subtract log(p) during sieve in order to take the
	 * number of prime powers into account
	 */
	mpfr_set_z(q, t, MPFR_RNDU);
	mpfr_log(q, q, MPFR_RNDU);

	/*
	 * even for very large numbers the float should be enough when in
	 * logarithmic scale
	 */
	qs->startup = mpfr_get_flt(q, MPFR_RNDU);

	mpz_clear(t);
	mpfr_clear(q);
}

static void
startup_stage(struct quadratic_sieve *qs, mpz_t n, ulong m)
{
	/* ceil square root of n to avoid negative numbers Q(x) */
	mpz_init(qs->n_sqrt);
	ceil_sqrt(n, qs->n_sqrt);

	qs->b = smooth(n, QUADRATIC_SIEVE_MAGIC);

	qs->m = m;
	qs->min_sieve = 0;
	qs->max_sieve = qs->m;
	qs->sieve = malloc(qs->m * sizeof(*qs->sieve));
	sieve_compute_startup(qs, n);

	qs->fb = factor_base_create(n, qs->n_sqrt, qs->b);

	qs->num_smooth = 0;
	qs->max_smooth = qs->fb->size + 1;
	qs->thresh = QUADRATIC_SIEVE_THRESH;

	/* create lookup storages when finding a solution */
	qs->smooth_relations = malloc(qs->max_smooth * sizeof(mpz_t));
	qs->least_p_residues = malloc(qs->max_smooth * sizeof(mpz_t));
	for (ulong i = 0; i < qs->max_smooth; i++) {
		mpz_init(qs->smooth_relations[i]);
		mpz_init(qs->least_p_residues[i]);
	}

	/* create matrix that will contain the exponent vectors (mod 2) */
	qs->A = matrix_create(qs->max_smooth, qs->fb->size);
	matrix_zero(qs->A);

	/*
	 * helper array that will help us identify linear dependencies after gauss
	 * elimination
	 */
	qs->pivots = malloc(qs->max_smooth * sizeof(int));
}

static void
sieve_stage(struct quadratic_sieve *qs)
{
	/* reset */
	for (ulong i = 0; i < qs->m; i++) {
		qs->sieve[i] = qs->startup;
	}

	struct factor_base *fb = qs->fb;
	/* perform sieve operations */
	for (ulong i = 0; i < fb->size; i++) {
		struct factor_prime *prime = fb->primes + i;
		for (ulong j = 0; j < 2; j++) {
			while (prime->r[j] < qs->max_sieve) {
				qs->sieve[prime->r[j] - qs->min_sieve] -= prime->value_log;
				prime->r[j] += prime->value;
			}

			/* skip second root for prime number 2 since it equals first root */
			if (prime->r[j] == 2) {
				break;
			}
		}
	}
}

/*
 * Note that the prime exponent is in mod 2, meaning that in the matrix the
 * cell will have a 0 for equal number of primes and 1 for odd number of
 * primes.
 */
static void
trial_division_stage(struct quadratic_sieve *qs, mpz_t n)
{
	mpz_t t;
	mpz_init(t);

	struct factor_base *fb = qs->fb;

	/*
	 * loop through the entire sieve array or until we have enough smooth
	 * numbers
	 */
	for (ulong i = 0; i < qs->m && qs->num_smooth < qs->max_smooth; i++) {
		if (qs->sieve[i] < qs->thresh) {
			/* possible smooth factorization */
			mpz_set_ui(t, qs->min_sieve + i);
			mpz_add(t, t, qs->n_sqrt);
			mpz_set(qs->least_p_residues[qs->num_smooth], t); /* store (n_sqrt + x) */
			mpz_mul(t, t, t);
			mpz_sub(t, t, n);
			mpz_set(qs->smooth_relations[qs->num_smooth], t); /* store Q(x)  = (n_sqrt + x)^2 - n */

			/* attempt to factorize number over factor base */
			for (ulong j = 0; j < fb->size; j++) {
				struct factor_prime *prime = fb->primes + j;
				ulong num_div = 0;
				while (mpz_divisible_ui_p(t, prime->value)) {
					mpz_divexact_ui(t, t, prime->value);
					num_div++;
				}
				/* store exponent vector mod 2 */
				if (num_div % 2) {
					matrix_set(qs->A, qs->num_smooth, j);
				}

				/* we are done, no point in trying more prime numbers */
				if (mpz_cmp_ui(t, 1) == 0) {
					qs->num_smooth++;
					break;
				}
			}

			if (mpz_cmp_ui(t, 1) != 0) {
				/* skip, was not a smooth factorization */
				matrix_zero_row(qs->A, qs->num_smooth);
			}
		}
	}

	mpz_clear(t);
}

/*
 * The linear algebra stage factor is not guaranteed to be a prime number but
 * can be a composite. It can also find more than one factors but which could
 * be a duplicate of earlier found factor, or a prime number appearing in a
 * earlier found composite factor and so on.
 *
 * This function will attempt to add factors in a controlled way so that no
 * redudant factors are added.
 */
static void
push_factor(mpz_t n, mpz_t t, struct factors *f)
{
	struct prime_sieve *ps = prime_sieve_create(MIN(TRIALDIVISION_LIMIT, mpz_get_ui(t)));

	factors_push(f, t);

	mpz_divexact(t, n, t);
	factors_push(f, t);

	while (factors_remove_composite(f, t) && trial_division(t, f, ps));

	prime_sieve_destroy(ps);
}

static int
linear_algebra_stage(struct quadratic_sieve *qs, mpz_t n, struct factors *factors)
{
	mpz_t t, x, y;
	mpz_inits(t, x, y, NULL);

	for (ulong i = 0; i < qs->A->rows; i++) {
		qs->pivots[i] = 0;
	}
	matrix_gauss(qs->A, qs->pivots);

	int factor_found = 0;
	/* for each row that was not used in the fast gauss over GF(2) */
	for (ulong i = 0; i < qs->max_smooth && !factor_found; i++) {
		if (!qs->pivots[i]) {
			/*
			 * iff this row is not zero, it has a linear combination with one
			 * or more rows
			 */
			mpz_set(x, qs->least_p_residues[i]);
			mpz_set(y, qs->smooth_relations[i]);
			for (ulong col = 0; col < qs->A->cols; col++) {
				if (matrix_get(qs->A, i, col)) {
					/*
					 * note that we will not multiply the same row more than
					 * once since we only multiply the rows that are included
					 * in the pivots which always contain one 1 per row
					 */
					for (ulong row = 0; row < qs->A->rows; row++) {
						if (qs->pivots[row] && matrix_get(qs->A, row, col)) {
							mpz_mul(x, x, qs->least_p_residues[row]);
							mpz_mul(y, y, qs->smooth_relations[row]);
						}
					}
				}
			}

			if (mpz_perfect_square_p(y)) {
				/* x - sqrt(y) mod n */
				mpz_sqrt(y, y);
				mpz_sub(t, x, y);
				mpz_mod(t, t, n);
				mpz_gcd(t, t, n);
				if (mpz_cmp(t, n) != 0 && mpz_cmp_ui(t, 1) != 0) {
					push_factor(n, t, factors);
					factor_found = 1;
				}
			}
		}
	}

	matrix_zero(qs->A);
	qs->num_smooth = 0;

	mpz_clears(t, x, y, NULL);

	return factor_found;
}

static void
cleanup_stage(struct quadratic_sieve *qs)
{
	mpz_clear(qs->n_sqrt);

	for (ulong i = 0; i < qs->max_smooth; i++) {
		mpz_clear(qs->least_p_residues[i]);
		mpz_clear(qs->smooth_relations[i]);
	}

	matrix_destroy(qs->A);
	factor_base_destroy(qs->fb);

	free(qs->pivots);
	free(qs->sieve);
	free(qs->least_p_residues);
	free(qs->smooth_relations);

	mpfr_free_cache();
}

int
quadratic_sieve(mpz_t n, struct factors *factors, ulong m)
{
	struct quadratic_sieve *qs;

	qs = malloc(sizeof(*qs));
	startup_stage(qs, n, m);

	int factor_found = 0;
	while (!factor_found) {
		sieve_stage(qs);
		trial_division_stage(qs, n);

		/*
		 * only proceed if we have enough smooth relations, otherwise skip and
		 * start sieve again
		 */
		if (qs->num_smooth == qs->max_smooth) {
			factor_found = linear_algebra_stage(qs, n, factors);
		}

		/* offset the interval for next attempt */
		qs->min_sieve += qs->m;
		qs->max_sieve += qs->m;
	}

	cleanup_stage(qs);
	free(qs);

	return factor_found;
}
