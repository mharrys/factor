#include "config.h"
#include "factors.h"
#include "primesieve.h"
#include "trialdivision.h"

#include "stdio.h"
#include "string.h"

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
	if (argc > 1) {
		strncpy(&line[0], argv[1], INTEGER_LIMIT);
	} else {
		scanf("%s\n", &line[0]);
	}

	mpz_t n;
	mpz_init(n);
	if (mpz_set_str(n, &line[0], 0) == -1) {
		fprintf(stderr, "factor: input must be an integer\n");
		mpz_clear(n);
		return 1;
	}

	if (mpz_probab_prime_p(n, MILLERRABIN_REPEATS) > 0) {
		gmp_printf("%Zd: %Zd\n", n, n);
		mpz_clear(n);
		return 0;
	}

	mpz_t t;
	mpz_init_set(t, n);

	struct factors *f = factors_create();
	struct prime_sieve *ps = prime_sieve_create(TRIALDIVISION_LIMIT);

	trial_division(t, f, ps);
	print_result(n, f);

	mpz_clears(n, t, NULL);
	factors_destroy(f);
	prime_sieve_destroy(ps);

	return 0;
}
