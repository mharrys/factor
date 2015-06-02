#include "smooth.h"

#include <stdio.h>
#include <stdlib.h>

ulong
smooth(mpz_t n, float c)
{
	mpz_t z;
	mpz_init(z);

	mpfr_t logn, loglogn, t;
	mpfr_inits(logn, loglogn, t, NULL);

	/* log(n) */
	mpfr_set_z(t, n, MPFR_RNDU);
	mpfr_log(logn, t, MPFR_RNDU);

	/* log(log(n)) */
	mpfr_log(loglogn, logn, MPFR_RNDU);

	/* log(n) * log(log(n)) */
	mpfr_mul(t, logn, loglogn, MPFR_RNDU);

	/* sqrt(log(n) * log(log(n))) */
	mpfr_sqrt(t, t, MPFR_RNDU);

	/* 0.5 * sqrt(log(n) * log(log(n))) */
	mpfr_mul_d(t, t, 0.5, MPFR_RNDU);

	/* e^(0.5 * sqrt(log(n) * log(log(n)))) */
	mpfr_exp(t, t, MPFR_RNDU);

	/* c * e^(0.5 * sqrt(log(n) * log(log(n)))) */
	mpfr_mul_d(t, t, c, MPFR_RNDU);

	mpfr_get_z(z, t, MPFR_RNDU);

	/*
	 * even for very large n the number should be small enough for ulong, but
	 * lets check in honor of murphys law
	 */
	ulong b = 0;
	if (!mpz_fits_ulong_p(z)) {
		fprintf(stderr, "smooth: can not fit smoothness bound into a ulong\n");
	} else {
		b = mpz_get_ui(z);
	}

	mpfr_clears(logn, loglogn, t, NULL);
	mpfr_free_cache();

	mpz_clear(z);

	return b;
}
