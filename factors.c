#include "factors.h"

#include <stdlib.h>

#define MEM_CHUNK 2	/* allocation multiplier */

struct factors *
factors_create()
{
	struct factors *f;

	f = malloc(sizeof(*f));
	f->size = 0;
	f->capacity = MEM_CHUNK;
	f->factors = realloc(f->factors, f->capacity * sizeof(mpz_t));

	return f;
}

void
factors_destroy(struct factors *f)
{
	for (int i = 0; i < f->size; i++) {
		mpz_clear(f->factors[i]);
	}
	free(f->factors);
	free(f);
}

static void
allocate_if_needed(struct factors *f)
{
	if (f->size == f->capacity) {
		f->capacity *= MEM_CHUNK;
		f->factors = realloc(f->factors, f->capacity * sizeof(mpz_t));
	}
}

void
factors_push(struct factors *f, mpz_t factor)
{
	allocate_if_needed(f);

	mpz_init(f->factors[f->size]);
	mpz_set(f->factors[f->size], factor);

	f->size++;
}

void
factors_pop(struct factors *f, mpz_t factor)
{
	factors_peek_end(f, factor);

	mpz_clear(f->factors[f->size - 1]);
	f->size--;
}

int
factors_remove_composite(struct factors *f, mpz_t factor)
{
	int i = 0;
	int composite = 0;

	for (; i < f->size; i++) {
		if (mpz_probab_prime_p(f->factors[i], MILLERRABIN_REPEATS) == 0) {
			mpz_set(factor, f->factors[i]);
			composite = 1;
			break;
		}
	}

	if (composite == 0) {
		return 0;
	}

	/*
	 * fill gap if factor was not located at the end
	 */
	for (; i < (f->size - 1); i++) {
		mpz_set(f->factors[i], f->factors[i + 1]);
	}

	mpz_clear(f->factors[f->size - 1]);
	f->size--;

	return 1;
}

void
factors_peek_front(struct factors *f, mpz_t front)
{
	mpz_set(front, f->factors[0]);
}

void
factors_peek_end(struct factors *f, mpz_t end)
{
	mpz_set(end, f->factors[f->size - 1]);
}

static int
compare(const void *a, const void *b)
{
	return mpz_cmp(*((mpz_t *) a), *((mpz_t *) b));
}

void
factors_sort(struct factors *f)
{
	qsort(f->factors, f->size, sizeof(mpz_t), compare);
}
