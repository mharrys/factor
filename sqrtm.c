#include "sqrtm.h"

#include <stdlib.h>

void
sqrtm(mpz_t r, mpz_t n, mpz_t p)
{
	if (mpz_divisible_p(n, p)) {
		mpz_set_ui(r, 0);
		return;
	}

	mpz_t c, t, q, z, a, b, bb;
	mpz_inits(c, t, q, z, a, b, bb, NULL);

	/* factor out powers of 2 from (p - 1) */
	mpz_sub_ui(t, p, 1);
	ulong s = 0;
	while (mpz_even_p(t)) {
		mpz_div_ui(t, t, 2);
		s++;
	}

	/* q = (p - 1) / 2^s */
	mpz_sub_ui(q, p, 1);
	mpz_ui_pow_ui(t, 2, s);
	mpz_div(q, q, t);

	/* select a z that is a quadratic non-residue modulo p */
	mpz_set_ui(z, 0);
	while (!(mpz_legendre(z, p) == -1)) {
		mpz_add_ui(z, z, 1);
	}

	mpz_powm(c, z, q, p); /* c = z^q mod p */

	/* r = n^((q+1)/2) mod p */
	mpz_add_ui(t, q, 1);
	mpz_div_ui(t, t, 2);
	mpz_powm(r, n, t, p);

	mpz_powm(a, n, q, p); /* a = n^q mod p */

	ulong m = s;
	while (1) {
		mpz_mod(t, a, p); /* a = 1 mod p */

		if (mpz_cmp_ui(t, 1) == 0) {
			break;
		}

		/* find 0 < i < m such that a^2^i = 1 mod p */
		ulong i;
		for (i = 0; i < m; i++) {
			mpz_ui_pow_ui(t, 2, i);
			mpz_powm(t, a, t, p);
			if (mpz_cmp_ui(t, 1) == 0) {
				break;
			}
		}

		/* b = c^2^(m-i-1) mod p */
		mpz_ui_pow_ui(t, 2, m - i - 1);
		mpz_powm(b, c, t, p);

		/* r = r * b mod p */
		mpz_mul(t, r, b);
		mpz_mod(r, t, p);

		/* a = a * b^2 mod p */
		mpz_mul(bb, b, b);
		mpz_mul(t, a, bb);
		mpz_mod(a, t, p);

		mpz_mod(c, bb, p); /* c = b^2 mod p */

		m = i;
	}

	mpz_clears(c, t, q, z, a, b, bb, NULL);
}
