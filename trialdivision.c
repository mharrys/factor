#include "trialdivision.h"

#include "factors.h"
#include "primesieve.h"

#include <stdlib.h>

int
trial_division(mpz_t n, struct factors *factors, struct prime_sieve *ps)
{
	if (mpz_cmp_ui(n, 1) <= 0) {
		return 0;
	}

	mpz_t f, p, pp;
	mpz_inits(f, p, pp, NULL);
	mpz_set(f, n);

	int factor_found = 0;
	for (ulong i = 0; i < ps->size; i++) {
		mpz_ui_pow_ui(pp, ps->primes[i], 2);

		if (mpz_cmp(pp, f) > 0) {
			break;
		}

		/* set f = f / p while f = 0 mod p */
		while (mpz_congruent_ui_p(f, 0, ps->primes[i])) {
			mpz_set_ui(p, ps->primes[i]);
			mpz_cdiv_q(f, f, p);

			factor_found = 1;
			factors_push(factors, p);
		}
	}

	if (mpz_cmp_ui(f, 1) > 0) {
		factors_push(factors, f);
	}

	mpz_clears(f, p, pp, NULL);

	return factor_found;
}
