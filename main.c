#include "config.h"
#include "factors.h"

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
		fprintf(stderr, "factor: could not parse input to an integer\n");
		return 1;
	}

	gmp_printf("%Zd\n", n);
	mpz_clear(n);

	return 0;
}
