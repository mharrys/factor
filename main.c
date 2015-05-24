#include "config.h"

#include "stdio.h"
#include "string.h"

#define INTEGER_LIMIT 255

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
