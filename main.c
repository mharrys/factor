#include "config.h"
#include "factors.h"
#include "primesieve.h"
#include "trialdivision.h"
#include "quadraticsieve.h"

#include <stdio.h>
#include <string.h>

static void
print_result(mpz_t n, struct factors *f)
{
	gmp_printf("%Zd: ", n);
	for (ulong i = 0; i < f->size; i++) {
		gmp_printf("%Zd ", f->factors[i]);
	}
	printf("\n");
}

int
main(int argc, char **argv)
{
	char line[INTEGER_LIMIT];
	if (argc > 1)
		strncpy(&line[0], argv[1], INTEGER_LIMIT);
	else if (scanf("%s\n", &line[0]) != 1) {
		fprintf(stderr, "factor: unable to read number from stdin\n");
		return 1;
	}

	mpz_t n;
	mpz_init(n);

	if (mpz_set_str(n, &line[0], 0) == -1 || mpz_cmp_ui(n, 1) < 0) {
		fprintf(stderr, "factor: input must be a positive integer\n");
		mpz_clear(n);
		return 1;
	}

	if (mpz_cmp_ui(n, 1) == 0 || mpz_probab_prime_p(n, MILLERRABIN_REPEATS) > 0) {
		gmp_printf("%Zd: %Zd\n", n, n);
		mpz_clear(n);
		return 0;
	}

	mpz_t t;
	mpz_init(t);
	mpz_sqrt(t, n);

	struct factors *f = factors_create();
	struct prime_sieve *ps = prime_sieve_create(MIN(TRIALDIVISION_LIMIT, mpz_get_ui(t)));

	if (mpz_perfect_square_p(n)) {
		factors_push(f, t);
		factors_push(f, t);
	} else {
		mpz_set(t, n);
		factors_push(f, t);
	}

	/* run trial division to find find small factors */
	while (factors_remove_composite(f, t) && trial_division(t, f, ps));
	prime_sieve_destroy(ps);

	/* run quadratic sieve until factorized into only prime numbers */
	while (factors_remove_composite(f, t) && quadratic_sieve(t, f, QUADRATIC_SIEVE_SIZE));

	factors_sort(f);
	print_result(n, f);

	mpz_clears(n, t, NULL);
	factors_destroy(f);

	return 0;
}
