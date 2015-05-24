#include "primesieve.h"

#include "math.h"
#include "stdlib.h"
#include "string.h"

struct prime_sieve *
prime_sieve_create(ulong n)
{
	struct prime_sieve *ps;

	ps = malloc(sizeof(*ps));
	ps->size = 0;
	ps->primes = NULL;

	if (n < 2) {
		return ps;
	}

	/* allocate on heap since the stack can not handle very large n */
	int *a = malloc(n * sizeof(int));

	memset(a, 1, n * sizeof(int));
	a[0] = 0;
	a[1] = 0;

	ulong m = ceil(sqrt(n));
	for (ulong i = 2; i < m; i++) {
		if (a[i]) {
			for (ulong j = i * i; j < n; j += i) {
				a[j] = 0;
			}
		}
	}

	for (ulong i = 2; i < n; i++) {
		if (a[i]) {
			ps->size++;
			ps->primes = realloc(ps->primes, ps->size * sizeof(ulong));
			ps->primes[ps->size - 1] = i;
		}
	}

	free(a);

	return ps;
}

void
prime_sieve_destroy(struct prime_sieve *ps)
{
	if (ps->size > 0) {
		free(ps->primes);
	}
	free(ps);
}
